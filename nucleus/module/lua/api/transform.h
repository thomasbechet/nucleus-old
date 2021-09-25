#ifndef NULUA_TRANSFORM_API_H
#define NULUA_TRANSFORM_API_H

#include <nucleus/nucleus.h>
#include <lua.h>

#define NULUA_TRANSFORM_TYPENAME "Transform"

nu_result_t nulua_register_transform_api(lua_State *L);

#endif