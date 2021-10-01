#ifndef NULUA_JSON_API_H
#define NULUA_JSON_API_H

#include <nucleus/nucleus.h>
#include <lua.h>

#define NULUA_JSON_TYPENAME "Json"

nu_result_t nulua_register_json_api(lua_State *L);

#endif