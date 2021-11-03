#include <nucleus/module/lua/api/json.h>

#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

int Json__call(lua_State *L)
{
    // nu_json_t *j = (nu_json_t*)lua_newuserdata(L, sizeof(nu_json_t));
    luaL_setmetatable(L, NULUA_JSON_TYPENAME);
    // nu_json_allocate_from_file(j, )

    // lua_pushlightuserdata()
    return 1;
}

nu_result_t nulua_register_json_api(lua_State *L)
{
    // static const luaL_Reg JsonObj_metatable[] = {
    //     {"__index",    Vec3Obj__index},
    //     {"__newindex", Vec3Obj__newindex},
    //     {NULL, NULL}
    // };
    // static const luaL_Reg Json_metatable[] = {
    //     {"__call", Json__call},
    //     {NULL, NULL}
    // };

    // /* create Json */
    // luaL_newlib(L, Json_methods);
    // luaL_newlib(L, Json_metatable);
    // lua_setmetatable(L, -2);
    // lua_setglobal(L, NULUA_JSON_TYPENAME);

    // /* object metatable */
    // luaL_newmetatable(L, NULUA_JSON_TYPENAME);
    // luaL_setfuncs(L, Vec3Obj_metatable, 0);
    // lua_pop(L, 1);

    return NU_SUCCESS;
}