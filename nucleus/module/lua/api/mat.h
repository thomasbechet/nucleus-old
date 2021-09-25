#ifndef NULUA_MAT_API_H
#define NULUA_MAT_API_H

#include <nucleus/nucleus.h>
#include <lua.h>

#define NULUA_MAT4_TYPENAME "Mat4"

nu_result_t nulua_register_mat_api(lua_State *L);

#endif