#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "config.h"
#include <poll.h>

#define MAX_CLIENTS 64
// #define INITIAL_CLIENTS 64
// #define CLIENT_TIMEOUT (20 * 1000) // 20 seconds
#define MAX_URI_LEN 512
#define DEFAULT_BACKLOG MAX_CLIENTS

typedef int32_t sock_t;

typedef struct {
    sock_t socket;
    HttpConfig cfg;
    uint32_t client_no;
    struct pollfd* clients;
} HttpServer;

extern HttpServer srv;

sock_t get_server_socket(uint16_t ipver, uint16_t port);

// Create socket, bind address and start listening, does not accept any clients
int http_server_init(const char* site);

// Start accepting clients
int http_server_start();

int http_server_free();


#endif /* HTTP_SERVER_H */
