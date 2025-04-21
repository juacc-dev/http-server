#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <string.h>

typedef struct {
    void* ptr;
    size_t n;
} array;

typedef struct {
    char* ptr;
    size_t len;
} string;

static inline char str_ends_in(string str, char c) {
    return str.ptr[str.len] == c;
}

static inline char* str_find(string str, char c) {
    return memchr(str.ptr, str.len, c);
}

#endif /* ARRAY_H */
