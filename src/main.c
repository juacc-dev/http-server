#include "server.h"
#include "structs/array.h"

#define DEFAULT_SITE "./site"

int main(int argc, char* argv[]) {
    char* site = argc == 2 ? argv[1] : DEFAULT_SITE;

    if(http_server_init(site))
        return 1;

    if(http_server_start())
        return 2;

    http_server_free();

    return 0;
}
