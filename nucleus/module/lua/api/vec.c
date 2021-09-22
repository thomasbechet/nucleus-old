#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

#include <nucleus/module/lua/common/debug.h>

static int vec3_index(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_touserdata(L, 1);
    const char *key = luaL_checkstring(L, 2);
    if (NU_MATCH(key, "x")) {
        lua_pushnumber(L, (*v)[0]);
        return 1;
    } else if (NU_MATCH(key, "y")) {
        lua_pushnumber(L, (*v)[1]);
        return 1;
    } else if (NU_MATCH(key, "z")) {
        lua_pushnumber(L, (*v)[2]);
        return 1;
    }

    return 0;
}

static int vec3_new(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    nu_vec3f_zero(*v);

    /* meta table */
    lua_newtable(L);
    lua_pushcfunction(L, vec3_index);
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);

    nulua_debug_stack(L);

    return 1;
}

static const luaL_Reg vec3_methods[] = {
    {"new", vec3_new},
    {NULL, NULL}
};

nu_result_t nulua_register_vec_api(lua_State *L)
{
    /* Vec3 = {} */
    lua_createtable(L, 0, 0);
    luaL_setfuncs(L, vec3_methods, 0);
    lua_setglobal(L, "Vec3");

    /* meta = {} */
    // luaL_newmetatable(L, "Vec3");
    // meta_id = lua_gettop(L);
    // luaL_setfuncs(L, vec3_meta, 0);

    /* meta.__index = methods */
    // luaL_newlib(L, vec3_methods);
    // lua_setfield(L, meta_id, "__index");

    /* meta.__metatable = meta */
    // luaL_newlib(L, vec3_meta);
    // lua_setfield(L, meta_id, "__metatable");

    /* Vec3.__metatable = meta */
    // lua_setmetatable(L, class_id);
    

    return NU_SUCCESS;
}