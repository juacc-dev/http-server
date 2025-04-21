#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <netdb.h>

#define FILES_DIR "site"

// send() _might_ not send all bytes, this function ensures it does.
int sendall(int fd, char* buf, ssize_t len, int flags);

// sendfile() _might_ not send all bytes, this function ensures it does.
int sendfileall(int fd_out, int fd_in, ssize_t len);

// Get sockaddr, IPv4 or IPv6:
static inline void* get_in_addr(struct sockaddr* sa) {
    if(sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*) sa)->sin_addr);

    else
        return &(((struct sockaddr_in6*) sa)->sin6_addr);
}


// Get used port, IPv4 or IPv6:
static inline uint16_t get_port(struct addrinfo* ai) {
    unsigned short port;

    // IPv4
    if(ai->ai_family == AF_INET)
        port = ((struct sockaddr_in*) ai->ai_addr)->sin_port;

    // IPv6
    else
        port = ((struct sockaddr_in6*) ai->ai_addr)->sin6_port;

    // Convert to host byte order.
    return ntohs(port);
}


// Show client's address.
// `ipstr` needs to be of length `INET6_ADDRSTRLEN`.
static inline char* get_ipstr(struct sockaddr_in6 addr, char* ipstr) {
    // May be IPv4 or IPv6
    inet_ntop(AF_INET6, get_in_addr((struct sockaddr*) &addr),
              ipstr, INET6_ADDRSTRLEN);

    return ipstr;
}

#endif /* HTTP_UTILS_H */
