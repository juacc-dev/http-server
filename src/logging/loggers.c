#include "logging.h"

#include <stdio.h>
#include <escape.h>
#include <errno.h>
#include <string.h>

#if defined(LOGGING_DEBUG)
#   define MSG_EXTRA " $c7(%s:%d)$r "
#else
#   define MSG_EXTRA " "
#endif

// #undef LOGGING_DEBUG

const char* log_prefix[] = {
    [LOGGING_LEVEL_DEBUG]   = "$c2$l$b[Debug]$r" MSG_EXTRA,
    [LOGGING_LEVEL_INFO]    = "$ch7$b[Info]$r" MSG_EXTRA,
    [LOGGING_LEVEL_WARNING] = "$c5$b[Warning]$r" MSG_EXTRA,
    [LOGGING_LEVEL_ERROR]   = "$c1$b[Error]$r" MSG_EXTRA,
};

int _log_printer(LOGGING_INTERNAL_PROTO) {
    va_list ap;
    int ret = 0;
    int err = errno;

    va_start(ap, message);

#if defined (LOGGING_DEBUG)
    ret += fprintfc(stderr, log_prefix[level], file, line);
#else
    ret += fprintc(stderr, log_prefix[level]);
#endif

    ret += vfprintfc(stderr, message, ap);

    va_end(ap);

    // If errno was set
    if(err != 0)
        ret += fprintfc(stderr, " :: $i$c1[perror] $c7(%d)$r $i%s", err, strerror(err));

    ret += fputc('\n', stderr);

    errno = 0;

    return ret;
}
