#ifndef HTTP_CONFIG_H
#define HTTP_CONFIG_H

#include <stdint.h>

typedef struct {
    const char* site;
    uint16_t port;
    uint16_t ipver;
    int32_t backlog;
} HttpConfig;

// extern HttpConfig cfg;

int parse_config(const char* site);

#endif /* HTTP_CONFIG_H */
