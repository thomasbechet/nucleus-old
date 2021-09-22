#include <nucleus/module/lua/api/context.h>

#include <lauxlib.h>

static int nulua_context_get_delta_time(lua_State *L)
{
    lua_Number delta = (double)nu_context_get_delta_time();
    lua_pushnumber(L, delta);
    return 1;
}
static int nulua_context_request_stop(lua_State *L)
{
    nu_context_request_stop();
    return 0;
}

static const struct luaL_Reg context_functions[] = {
    {"get_delta_time", nulua_context_get_delta_time},
    {"request_stop",   nulua_context_request_stop},
    {NULL, NULL}
};

nu_result_t nulua_register_context_api(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, context_functions, 0);
    lua_setglobal(L, "Context");
    return NU_SUCCESS;
}