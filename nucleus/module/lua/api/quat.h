#ifndef NULUA_QUAT_API_H
#define NULUA_QUAT_API_H

#include <nucleus/nucleus.h>
#include <lua.h>

#define NULUA_QUAT_TYPENAME "Quat"

int Quat_new_0(lua_State *L);

nu_result_t nulua_register_quat_api(lua_State *L);

#endif