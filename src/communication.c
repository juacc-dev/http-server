#include "server.h"
#include "config.h"
#include "communication.h"
#include "utils.h"
#include "logging/logging.h"
#include "structs/array.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>

#define MAX_REQUEST_LEN 4096
// #define MAX_PATH_LEN (MAX_URI_LEN + sizeof FILES_DIR + sizeof ".html")
#define MAX_METHOD_LEN 8
#define MAX_VER_LEN 10
#define MAX_URI_LEN_STR "2048"
#define MAX_METHOD_LEN_STR "8"
#define MAX_VER_LEN_STR "10"

#define CRLF "\r\n"
#define MAX_RESPONSE_BUFSIZE 256

#define DONT_SEND_FILE -1

const char* http_statuses[] = {
    [HTTP_STATUS_CONTINUE] = "Continue",

    [HTTP_STATUS_OK]         = "OK",
    [HTTP_STATUS_ACCEPTED]   = "Accepted",
    [HTTP_STATUS_NO_CONTENT] = "No Content",

    [HTTP_STATUS_MOVED_PERM] = "Moved Permanently",

    [HTTP_STATUS_BAD_REQUEST] = "Bad Request",
    [HTTP_STATUS_NOT_FOUND]   = "Not Found",
    [HTTP_STATUS_BAD_METHOD]  = "Bad Method",
    [HTTP_STATUS_TIMEOUT]     = "Request Timed Out",
    [HTTP_STATUS_LEGAL]       = "Unavailable For Legal Reasons",

    [HTTP_STATUS_SERVER_ERR]  = "Internal Server Error",
    [HTTP_STATUS_UNAVAILABLE] = "Page Unavailable",
};

// Turn a string (e.g. "GET") into an enum HttpMethod (e.g. HTTP_METHOD_GET)
// NEED A HASH MAP
static inline enum HttpMethod parse_method(char* str) {
    if(!strcmp(str, "GET"))
        return HTTP_METHOD_GET;
    else if(!strcmp(str, "HEAD"))
        return HTTP_METHOD_HEAD;
    // else if(!strcmp(str, "POST"))
    //     return HTTP_METHOD_POST;

    // Most methods not supported hehe
    return HTTP_METHOD_INVALID;
}

// Here is where lua should come to help
// NEED A HASH MAP
int parse_URI(char* uri, char* path) {
    // Cut string before any '?'
    char* qmark = strrchr(uri, '?');
    if(qmark)
        *qmark = '\0';

    // Add a slash at the end if there is no extension
    // "whatever" -> "whatever/"
    if(!strrchr(uri, '.') && uri[strlen(uri) - 1] != '/')
        strcat(uri, "/");

    // Transform "whatever/" -> "whatever/index.html"
    if(uri[strlen(uri) - 1] == '/')
        strcat(uri, "index.html");

    // Prepend path
    strcpy(path, srv.cfg.site);
    strcat(path, uri[0] == '/' ? "/pages" : "/pages/");
    strcat(path, uri);

    // const char* dot = strrchr(path, '.');
    // if(!dot || dot == path) {
    //     strcat(path, ".html");
    //     return;
    // }
    return 0;
}

int http_recv(sock_t client, HttpRequest* req) {
    char message[MAX_REQUEST_LEN + 1], method[MAX_METHOD_LEN + 1],
        http_ver[MAX_VER_LEN + 1];

    int r = recv(client, message, sizeof message, 0);

    switch(r) {
        case -1:
            log_error("recv() failed.");
            return -1;

        case 0:
            log_debug("Client has closed the connection.");
            return 0;
    }

    // Parse something like "GET /some_uri HTTP/1.1"
    sscanf(message,
           "%" MAX_METHOD_LEN_STR "s "
           "%" MAX_URI_LEN_STR "s "
           "%" MAX_VER_LEN_STR "s",
           method, req->uri, http_ver);

    // Only HTTP 1.1 supported
    if(strcmp(http_ver, "HTTP/1.1")) {
        log_error("Bad version: %s.", http_ver);
        return -1;
    }

    req->method = parse_method(method);

    log_debug("Client: $c7%s %s$r.", method, req->uri);

    return r;
}

int http_craft(HttpRequest req, HttpResponse* res) {
    char path[PATH_MAX];

    parse_URI(req.uri, path);

    if((res->fd = open(path, O_RDONLY)) == -1) {
        errno = 0;
        log_debug("404 Not Found.");
        res->status      = HTTP_STATUS_NOT_FOUND;
        res->fields.mime = MIME_TEXT_HTML;
        res->fields.len  = 0;
        return -1;
    }

    res->status = HTTP_STATUS_OK;

    // File type
    res->fields.mime = get_mime_type(path);

    // File size
    struct stat fs;
    fstat(res->fd, &fs);
    res->fields.len = fs.st_size;

    if(req.method == HTTP_METHOD_HEAD && res->status == HTTP_STATUS_OK) {
        close(res->fd);
        res->fd = DONT_SEND_FILE;
    }

    return 0;
}

// Receive a response object.
int http_send(sock_t client, HttpResponse* res) {
    // Craft header
    char header[MAX_RESPONSE_BUFSIZE];

    int l = snprintf(header, MAX_RESPONSE_BUFSIZE,
                     "HTTP/1.1 %d %s" CRLF
                     "Content-Length: %d" CRLF
                     "Connection: Keep-Alive" CRLF,
                     res->status, http_status(res->status), res->fields.len);

    if(res->fields.mime != MIME_NO_TYPE)
        l += sprintf(header + l, "Content-Type: %s" CRLF,
                     mime_str(res->fields.mime));

    strcat(header, CRLF);
    l += strlen(CRLF);

    // MSG_MORE so that is nothing is sent in this call
    int flag = res->fd == DONT_SEND_FILE ? 0 : MSG_MORE;

    // log_debug("Server: %d, len: %d\n", res->status, res->fields.len);

    sendall(client, header, l, flag);

    if(res->fd != DONT_SEND_FILE) {
        // Message is a file. Now everything is sent together
        sendfileall(client, res->fd, res->fields.len);

        close(res->fd);
    }

    return 0;
}
