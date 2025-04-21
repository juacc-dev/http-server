#include "server.h"
#include "logging/logging.h"
#include "config.h"
#include <stdint.h>
#include <string.h>
#include <linux/limits.h>
#include <luajit-2.1/lua.h>
#include <luajit-2.1/lualib.h>
#include <luajit-2.1/lauxlib.h>

static int get_field_int(lua_State* L, const char* k) {
    lua_pushstring(L, k);

    lua_gettable(L, -2);

    if(!lua_isnumber(L, -1)) {
        log_error("Failed to read config: $c7%s.", lua_tostring(L, -1));
        return -1;
    }

    int v = (int) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return v;
}

// static int get_field_str(lua_State* L, const char* k, char* v) {
//     lua_pushstring(L, k);

//     lua_gettable(L, -2);

//     if(!lua_isstring(L, -1)) {
//         log_error("Failed to read config: $c7%s.", lua_tostring(L, -1));
//         return -1;
//     }

//     strncpy(v, lua_tostring(L, -1), 32);
//     lua_pop(L, 1);

//     return 0;
// }

int parse_config(const char* site) {
    char config[PATH_MAX] = {0};
    strcat(config, site);
    strcat(config, "/config.lua");

    lua_State* L = lua_open();
    // luaL_openlibs(L);

    if(luaL_loadfile(L, config) || lua_pcall(L, 0, 0, 0)) {
        log_error("Failed to read config: $c7%s.", lua_tostring(L, -1));
        return 1;
    }

    lua_getglobal(L, "Server");

    if(!lua_istable(L, -1)) {
        log_error("Failed to read config: $c7%s.", lua_tostring(L, -1));
        return 2;
    }

    srv.cfg.site    = site;
    srv.cfg.port    = (uint16_t) get_field_int(L, "port");
    srv.cfg.ipver   = (uint16_t) get_field_int(L, "ipver");
    srv.cfg.backlog = get_field_int(L, "backlog");

    return 0;
}
