#include "server.h"
#include "logging/logging.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>

// Self-explanatory
sock_t get_server_socket(uint16_t ipver, uint16_t port) {
    sock_t server;

    // getaddrinfo() spects port number as a string
    char port_str[6];
    snprintf(port_str, 6, "%hu", port);

    struct addrinfo hints = {0}, *addr_results;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_family   = ipver == 6 ? AF_INET6 : AF_INET;
    hints.ai_flags    = AI_PASSIVE;  // Make bind() fill the nodename (i.e. IP)

    // `addr_results` will point to a linked list of options for connection.
    // One of them shall be chosen later.
    int info_status = getaddrinfo(NULL, port_str, &hints, &addr_results);

    if(info_status != 0)
        log_error("getaddrinfo(): %s.", gai_strerror(info_status));

    struct addrinfo* tmp;

    for(tmp = addr_results; tmp != NULL; tmp = tmp->ai_next) {
        server = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

        if(server == -1) {
            log_warning("Could not create socket with $lfamily %d, socktype %d "
                        "and protocol %d.",
                        tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

            continue;
        }

        // Reuse the port. Useful if always using the same port.
        if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (int[]){1}, sizeof(int)) ==
           -1)
            log_warning("Failed to set socket option SO_REUSEADDR.");

        // Bind name (address) to socket
        if(bind(server, tmp->ai_addr, tmp->ai_addrlen) == -1) {
            log_warning("Could not bind to result.");
            close(server);

            continue;
        }

        // Just to check that the used port and ipver are the ones specified
        uint16_t port     = get_port(tmp);
        char* used_ip_ver = tmp->ai_family == AF_INET ? "IPv4" : "IPv6";
        log_info("Listening on port $b%hu$r over %s.", port, used_ip_ver);

        break;
    }

    if(server == 0)
        log_error("Failed to bind anything!");

    freeaddrinfo(addr_results);

    return server;
}

