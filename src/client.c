#include "server.h"
#include "utils.h"
#include "client.h"
#include "logging/logging.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

int http_accept_client() {
    // Client c;
    struct sockaddr_in6 addr;
    socklen_t addr_len;

    sock_t client = accept(srv.socket, (struct sockaddr*) &addr, &addr_len);
    log_debug("Client number $c5%d$r.", client);

    if(client == -1) {
        log_error("Failed to accept connection.");
        return -1;
    }

    char ipstr[INET6_ADDRSTRLEN];
    log_info("Accepted connection from $b%s$r.", get_ipstr(addr, ipstr));

    int n = 0;
    while(n < MAX_CLIENTS && srv.clients[n].fd != -2)
        n++;

    srv.clients[n].fd     = client;
    srv.clients[n].events = POLLIN;

    return n;
}

// int http_recv_request(sock_t client, HttpRequest* req) {
//     char response[RESPONSE_MAX_SIZE + 1], method[MAX_METHOD_LEN + 1],
//         http_ver[MAX_VER_LEN + 1];

//     ssize_t r = recv(client, response, sizeof response, 0);

//     switch(r) {
//         case -1:
//             log_error("recv() failed.");
//             return -1;

//         case 0:
//             log_debug("Client has closed connection.");
//             return 0;
//     }

//     sscanf(response,
//            "%" MAX_METHOD_LEN_STR "s "
//            "%" MAX_URI_LEN_STR "s "
//            "%" MAX_VER_LEN_STR "s",
//            method, req->uri, http_ver);

//     if(strcmp(http_ver, "HTTP/1.1")) {
//         log_error("Bad version: %s.", http_ver);
//         return -1;
//     }

//     log_debug("Client wants $c7%s$r.", req->uri);

//     req->method = parse_method(method);

//     return r;
// }

// Resource get_server_resource(HttpRequest* req) {
//     Resource res;

//     char path[MAX_PATH_LEN];
//     parse_URI(req->uri, path);

//     if((res.fd = open(path, O_RDONLY)) == -1) {
//         // File does not exist
//         res.status = HTTP_STATUS_NOT_FOUND;
//         res.mime   = MIME_TEXT_HTML;
//         res.fd     = open(FILES_DIR "/internal/not_found/index.html", O_RDONLY);

//         return res;
//     }

//     res.status = HTTP_STATUS_OK;
//     res.mime   = get_mime_type(path);

//     return res;
// }

// int http_send_response(sock_t client, Resource res) {
//     char header[MAX_RESPONSE_BUFSIZE];

//     // Get file stats for `fd`, I only need its size
//     struct stat fs;
//     fstat(res.fd, &fs);

//     int length =
//         snprintf(header, MAX_RESPONSE_BUFSIZE,

//                  "HTTP/1.1 %d %s" CRLF "Content-Type: %s" CRLF
//                  "Content-Length: %ld" CRLF "Connection: Keep-Alive" CRLF CRLF,

//                  res.status, http_statuses[res.status], mime_to_str[res.mime],
//                  fs.st_size);

//     sendall(client, header, length, MSG_MORE);

//     sendfileall(client, res.fd, fs.st_size);

//     close(res.fd);

//     return 0;
// }
