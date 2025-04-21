#include "utils.h"
#include "logging/logging.h"

#include <stdint.h>
#include <string.h>
#include <sys/sendfile.h>

// send() _might_ not send all bytes.
// TODO: maybe could use MSG_MORE flag (send(2)).
int sendall(int fd, char* buf, ssize_t len, int flags) {
    ssize_t n;
    ssize_t sent = 0;

    do {
        n = send(fd, buf + sent, len - sent, flags);

        sent += n;

    } while(sent < len && n > 0);

    if(n != sent) {
        log_debug("Sent buffer by parts. (n = %ld, sent = %ld)", n, sent);
    }
    
    if(n == -1) {
        log_debug("Sent file by parts. (n = %ld, sent = %ld)", n, sent);
        return -1;
    }

    return sent;
}

// Same as sendall but optimized for zero-copy
// Not portable, should be faster tho.
int sendfileall(int fd_out, int fd_in, ssize_t len) {
    ssize_t n;
    ssize_t sent = 0;

    do {
        n = sendfile(fd_out, fd_in, &sent, len - sent);

    } while(sent < len && n != -1);

    if(n != sent) {
        log_debug("Sent file by parts.");
    }
    

    if(n == -1) {
        log_error("Failed to send file (%ld B sent of %ld B)\n", sent, len);
        return -1;
    }

    return sent;
}
