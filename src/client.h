#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

// Accept one connection
int http_accept_client();

static inline int http_close_connection(struct pollfd* client) {
    // Let's be nice
    shutdown(client->fd, SHUT_RDWR);
    close(client->fd);

    client->fd = -2;

    return 0;
}
