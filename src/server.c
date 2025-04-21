#include "server.h"
#include "communication.h"
#include "config.h"
#include "client.h"
#include "logging/logging.h"
#include <stdint.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <escape.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <pthread.h>

HttpServer srv;

// Returns a listening socket
int http_server_init(const char* site) {
    // Read lua config
    parse_config(site);

    log_info("Initializing server.");

    srv.socket    = get_server_socket(srv.cfg.ipver, srv.cfg.port);
    srv.clients   = malloc(MAX_CLIENTS * sizeof(struct pollfd));
    srv.client_no = MAX_CLIENTS;

    // memset(srv.clients, -2, MAX_CLIENTS * sizeof(struct pollfd));
    for(int i = 0; i < MAX_CLIENTS; i++)
        srv.clients[i].fd = -2;

    if(listen(srv.socket, srv.cfg.backlog) == -1) {
        log_error("Failed to listen for connections.");

        return -1;
    }

    log_debug("Listening for incoming connections.");

    srv.clients[0].fd     = srv.socket;
    srv.clients[0].events = POLLIN;

    return 0;
}

int http_server_start() {
    bool connection_keep_alive = true;

    while(connection_keep_alive) {
        int poll_nevents = poll(srv.clients, srv.client_no, -1);

        if(poll_nevents == -1) {
            log_error("Unexpected error occurred with poll()");
            return -1;
        }

        int i = -1;
        while(++i < MAX_CLIENTS && poll_nevents > 0) {
            int pollin_happened = srv.clients[i].revents & POLLIN;

            if(!pollin_happened)
                continue;

            poll_nevents--;

            if(i == 0) {
                // index 0 is the server socket, i.e. there's a new client
                http_accept_client();
                continue;
            }

            // Communication with client
            HttpRequest req;
            HttpResponse res;

            switch(http_recv(srv.clients[i].fd, &req)) {
                default: {
                    http_craft(req, &res);

                    http_send(srv.clients[i].fd, &res);

                    break;
                }

                case 0:
                    log_debug("Closing connection with client.");

                    http_close_connection(&srv.clients[i]);
                    break;

                case 1:
                    log_info("Bad request.");

                    break;
            }
        }
    }

    return 0;
}

int http_server_free() {
    log_info("Terminating server.");

    close(srv.socket);
    free(srv.clients);

    return 0;
}
