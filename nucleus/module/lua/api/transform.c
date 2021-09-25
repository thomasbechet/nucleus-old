#include <nucleus/module/lua/api/transform.h>

#include <nucleus/module/lua/api/vec.h>
#include <nucleus/module/lua/api/quat.h>
#include <nucleus/module/lua/api/mat.h>

#include <lauxlib.h>

static int TransformObj_forward(lua_State *L)
{
    lua_getfield(L, 1, "rotation");
    nu_quatf_t *rotation = (nu_quatf_t*)luaL_checkudata(L, -1, NULUA_QUAT_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_transform_build_direction(*rotation, NU_VEC3F_FORWARD, *v);
    return 1;
}
static int TransformObj_backward(lua_State *L)
{
    lua_getfield(L, 1, "rotation");
    nu_quatf_t *rotation = (nu_quatf_t*)luaL_checkudata(L, -1, NULUA_QUAT_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_transform_build_direction(*rotation, NU_VEC3F_BACKWARD, *v);
    return 1;
}
static int TransformObj_left(lua_State *L)
{
    lua_getfield(L, 1, "rotation");
    nu_quatf_t *rotation = (nu_quatf_t*)luaL_checkudata(L, -1, NULUA_QUAT_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_transform_build_direction(*rotation, NU_VEC3F_LEFT, *v);
    return 1;
}
static int TransformObj_right(lua_State *L)
{
    lua_getfield(L, 1, "rotation");
    nu_quatf_t *rotation = (nu_quatf_t*)luaL_checkudata(L, -1, NULUA_QUAT_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_transform_build_direction(*rotation, NU_VEC3F_RIGHT, *v);
    return 1;
}
static int TransformObj_up(lua_State *L)
{
    lua_getfield(L, 1, "rotation");
    nu_quatf_t *rotation = (nu_quatf_t*)luaL_checkudata(L, -1, NULUA_QUAT_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_transform_build_direction(*rotation, NU_VEC3F_UP, *v);
    return 1;
}
static int TransformObj_down(lua_State *L)
{
    lua_getfield(L, 1, "rotation");
    nu_quatf_t *rotation = (nu_quatf_t*)luaL_checkudata(L, -1, NULUA_QUAT_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_transform_build_direction(*rotation, NU_VEC3F_DOWN, *v);
    return 1;
}
static int TransformObj_matrix(lua_State *L)
{
    lua_getfield(L, 1, "translation");
    nu_vec3f_t *translation = (nu_vec3f_t*)luaL_checkudata(L, -1, NULUA_VEC3_TYPENAME);
    lua_getfield(L, 1, "rotation");
    nu_quatf_t *rotation = (nu_quatf_t*)luaL_checkudata(L, -1, NULUA_QUAT_TYPENAME);
    lua_getfield(L, 1, "scale");
    nu_vec3f_t *scale = (nu_vec3f_t*)luaL_checkudata(L, -1, NULUA_VEC3_TYPENAME);
    
    nu_mat4f_t *m = (nu_mat4f_t*)lua_newuserdata(L, sizeof(nu_mat4f_t));
    luaL_setmetatable(L, NULUA_MAT4_TYPENAME);

    nu_transform_build_matrix(*translation, *rotation, *scale, *m);

    return 1;
}
static int Transform_new(lua_State *L)
{
    lua_newtable(L);
    luaL_setmetatable(L, NULUA_TRANSFORM_TYPENAME);

    Vec3_new_0(L);
    lua_setfield(L, -2, "translation");
    Quat_new_0(L);
    lua_setfield(L, -2, "rotation");
    Vec3_new_0(L);
    nu_vec3f_t *scale = (nu_vec3f_t*)luaL_checkudata(L, -1, NULUA_VEC3_TYPENAME);
    nu_vec3f_copy((nu_vec3f_t){1, 1, 1}, *scale);
    lua_setfield(L, -2, "scale");

    return 1;
}
static int Transform__call(lua_State *L)
{
    return Transform_new(L);
}

nu_result_t nulua_register_transform_api(lua_State *L)
{
    static const luaL_Reg TransformObj_methods[] = {
        {"forward",  TransformObj_forward},
        {"backward", TransformObj_backward},
        {"left",     TransformObj_left},
        {"right",    TransformObj_right},
        {"up",       TransformObj_up},
        {"down",     TransformObj_down},
        {"matrix",   TransformObj_matrix},
        {NULL, NULL}
    };
    static const luaL_Reg TransformObj_metatable[] = {
        {NULL, NULL}
    };
    static const luaL_Reg Transform_methods[] = {
        {NULL, NULL}
    };
    static const luaL_Reg Transform_metatable[] = {
        {"__call", Transform__call},
        {NULL, NULL}
    };

    /* create Transform */
    luaL_newlib(L, Transform_methods);
    luaL_newlib(L, Transform_metatable);
    lua_setmetatable(L, -2);
    lua_setglobal(L, NULUA_TRANSFORM_TYPENAME);

    /* object metatable */
    luaL_newmetatable(L, NULUA_TRANSFORM_TYPENAME);
    luaL_setfuncs(L, TransformObj_metatable, 0);
    luaL_newlib(L, TransformObj_methods);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);

    return NU_SUCCESS;
}