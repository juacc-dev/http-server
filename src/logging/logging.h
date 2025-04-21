#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#define LOGGING_DEBUG

enum LoggingLevel {
    LOGGING_LEVEL_INVALID = -1,
    LOGGING_LEVEL_DEBUG   = 0,
    LOGGING_LEVEL_INFO    = 1,
    LOGGING_LEVEL_WARNING = 2,
    LOGGING_LEVEL_ERROR   = 3,
};

// Sets logging level, returns previous level on success and
// `LOGGING_LEVEL_INVALID` on failure. Thread-safe
enum LoggingLevel log_set_level(enum LoggingLevel level);

// Gets current logging level. On failure returns `LOGGING_LEVEL_INVALID`.
// Thread-safe
enum LoggingLevel log_get_level();

// Initialize logging. `fds` is an array of open file descriptor of lenght
// `num_fds`. Returns 0 on success, -1 on failure.
int log_init(int* fds, ssize_t num_fds, enum LoggingLevel level);

// GNU extension to help with type checking in format specifiers.
#if defined(LOGGING_DEBUG)
#   define LOGGING_FUNC_PROTO \
    const char *file, const int line, const char *message, ...

#   define LOGGING_INTERNAL_PROTO \
        enum LoggingLevel level, const char* file, const int line, const char* message, ...

#   if defined(__GNUC__)
#       define LOGGING_FUNC_TYPE __attribute__((format(printf, 4, 5))) int

#   define LOGGING_DEF_FUNC(name, level) \
        (log_get_level > level ? \
            LOGGING_LEVEL_INVALID : \
            _log_printer(level, ))

#   else
#       define LOGGING_FUNC_TYPE int

#   endif

#else
#   define LOGGING_FUNC_PROTO const char *message, ...

#   define LOGGING_INTERNAL_PROTO \
        enum LoggingLevel level, const char* message, ...

#   if defined(__GNUC__)
#       define LOGGING_FUNC_TYPE __attribute__((format(printf, 2, 3))) int

#   else
#       define LOGGING_FUNC_TYPE int
#   endif
#endif

#if defined(LOGGING_DEBUG)
#   define LOGGING_DEBUG_MACRO_PROTO __FILE__, __LINE__,
#else
#   define LOGGING_DEBUG_MACRO_PROTO
#endif

LOGGING_FUNC_TYPE _log_printer(LOGGING_INTERNAL_PROTO);

#define log_debug(...) \
    (log_get_level() > LOGGING_LEVEL_DEBUG ? \
        -1 : \
        _log_printer(LOGGING_LEVEL_DEBUG, LOGGING_DEBUG_MACRO_PROTO __VA_ARGS__))

#define log_info(...) \
    (log_get_level() > LOGGING_LEVEL_INFO ? \
        -1 : \
        _log_printer(LOGGING_LEVEL_INFO, LOGGING_DEBUG_MACRO_PROTO __VA_ARGS__))

#define log_warning(...) \
    (log_get_level() > LOGGING_LEVEL_WARNING ? \
        -1 : \
        _log_printer(LOGGING_LEVEL_WARNING, LOGGING_DEBUG_MACRO_PROTO __VA_ARGS__))

#define log_error(...) \
    (log_get_level() > LOGGING_LEVEL_ERROR ? \
        -1 : \
        _log_printer(LOGGING_LEVEL_ERROR, LOGGING_DEBUG_MACRO_PROTO __VA_ARGS__))

#endif /* LOGGING_H */
