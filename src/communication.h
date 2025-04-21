#ifndef HTTP_COMUNICATION_H
#define HTTP_COMUNICATION_H

#include "server.h"
#include "mime/mime_types.h"
#include <stdint.h>

enum HttpMethod {
    HTTP_METHOD_GET,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_CONNECT,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_TRACE,
    HTTP_METHOD_PATCH,
    HTTP_METHOD_INVALID,
};

enum HttpStatusCode {
    HTTP_STATUS_CONTINUE = 100,

    HTTP_STATUS_OK         = 200,
    HTTP_STATUS_ACCEPTED   = 202,
    HTTP_STATUS_NO_CONTENT = 204,

    HTTP_STATUS_MOVED_PERM = 301,

    HTTP_STATUS_BAD_REQUEST = 400,
    HTTP_STATUS_NOT_FOUND   = 404,
    HTTP_STATUS_BAD_METHOD  = 405,
    HTTP_STATUS_TIMEOUT     = 408,
    HTTP_STATUS_LEGAL       = 451,

    HTTP_STATUS_SERVER_ERR  = 500,
    HTTP_STATUS_UNAVAILABLE = 503,
    HTTP_STATUS_VERSION_ERR = 505,
};

// Header fields for HttpRequest
typedef struct {
} HttpRequestHF;

// Header fields for HttpResponse
typedef struct {
} HttpResponseHF;

typedef struct {
    enum HttpMethod method;
    const char uri[MAX_URI_LEN];
    int16_t path, query, frag;
    HttpRequestHF fields;
} HttpRequest;

typedef struct {
    int32_t len;
    enum MimeType mime;
} HttpHeaderFields;

typedef struct {
    enum HttpStatusCode status;
    int fd;
    HttpHeaderFields fields;
} HttpResponse;

// Map between status code and status message
extern const char* http_statuses[];
static inline const char* http_status(enum HttpStatusCode code) {
    return http_statuses[code];
}

// Receive a request object.
int http_recv(sock_t client, HttpRequest* req);

// Read a request and craft a response.
int http_craft(HttpRequest req, HttpResponse* res);

// Receive a response object.
int http_send(sock_t client, HttpResponse* res);

#endif /* HTTP_COMUNICATION_H */
