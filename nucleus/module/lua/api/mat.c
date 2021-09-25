#include <nucleus/module/lua/api/mat.h>

#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

static int Mat4Obj__mul(lua_State *L)
{
    nu_mat4f_t *m1 = luaL_checkudata(L, 1, NULUA_MAT4_TYPENAME);

    nu_mat4f_t *m2;
    nu_vec4f_t *v4;
    nu_vec3f_t *v3;
    if ((m2 = luaL_testudata(L, 2, NULUA_MAT4_TYPENAME))) {
        nu_mat4f_t *m = (nu_mat4f_t*)lua_newuserdata(L, sizeof(nu_mat4f_t));
        luaL_setmetatable(L, NULUA_MAT4_TYPENAME);
        nu_mat4f_mul(*m1, *m2, *m);
        return 1;
    } else if ((v4 = luaL_testudata(L, 2, NULUA_VEC4_TYPENAME))) {
        nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
        luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
        nu_mat4f_mulv(*m1, *v4, *v);
        return 1;
    } else if ((v3 = luaL_testudata(L, 2, NULUA_VEC3_TYPENAME))) {
        nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
        luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
        nu_mat4f_mulv3(*m1, *v3, 1.0f, *v);
        return 1;
    }
    return 0;
}
int Mat4_new_0(lua_State *L)
{
    nu_mat4f_t *m = (nu_mat4f_t*)lua_newuserdata(L, sizeof(nu_mat4f_t));
    luaL_setmetatable(L, NULUA_MAT4_TYPENAME);
    nu_mat4f_identity(*m);
    return 1;
}
static int Mat4__call(lua_State *L)
{
    return Mat4_new_0(L);
}

nu_result_t nulua_register_mat_api(lua_State *L)
{
    static const luaL_Reg Mat4Obj_metatable[] = {
        {"__mul", Mat4Obj__mul},
        {NULL, NULL}
    };
    static const luaL_Reg Mat4_methods[] = {
        {NULL, NULL}
    };
    static const luaL_Reg Mat4_metatable[] = {
        {"__call", Mat4__call},
        {NULL, NULL}
    };

    /* create Mat4 */
    luaL_newlib(L, Mat4_methods);
    luaL_newlib(L, Mat4_metatable);
    lua_setmetatable(L, -2);
    lua_setglobal(L, NULUA_MAT4_TYPENAME);

    /* object metatable */
    luaL_newmetatable(L, NULUA_MAT4_TYPENAME);
    luaL_setfuncs(L, Mat4Obj_metatable, 0);
    lua_pop(L, 1);

    return NU_SUCCESS;
}