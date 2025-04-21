#include "logging.h"
#include <string.h>

static enum LoggingLevel logging_level;

int* logging_fds[16] = {0};

int log_init(int* fds, ssize_t num_fds, enum LoggingLevel level) {
    memcpy(logging_fds, fds, num_fds);

    log_set_level(level);

    return 0;
}

// TODO: thread safety
enum LoggingLevel log_set_level(enum LoggingLevel level) {
    enum LoggingLevel old_level = logging_level;

    logging_level = level;

    return old_level;
}

// TODO: thread safety
enum LoggingLevel log_get_level() { return logging_level; }
