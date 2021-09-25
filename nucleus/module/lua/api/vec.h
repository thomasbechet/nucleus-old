#ifndef NULUA_VEC_API_H
#define NULUA_VEC_API_H

#include <nucleus/nucleus.h>
#include <lua.h>

#define NULUA_VEC3_TYPENAME "Vec3"
#define NULUA_VEC4_TYPENAME "Vec4"

int Vec3_new_0(lua_State *L);
int Vec4_new_0(lua_State *L);

nu_result_t nulua_register_vec_api(lua_State *L);

#endif