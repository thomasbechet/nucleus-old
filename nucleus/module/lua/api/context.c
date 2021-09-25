#include <nucleus/module/lua/api/context.h>

#include <lauxlib.h>

static int Context_get_delta_time(lua_State *L)
{
    lua_Number delta = (double)nu_context_get_delta_time();
    lua_pushnumber(L, delta);
    return 1;
}
static int Context_request_stop(lua_State *L)
{
    nu_context_request_stop();
    return 0;
}

nu_result_t nulua_register_context_api(lua_State *L)
{
    static const struct luaL_Reg Context_methods[] = {
        {"get_delta_time", Context_get_delta_time},
        {"request_stop",   Context_request_stop},
        {NULL, NULL}
    };

    luaL_newlib(L, Context_methods);
    lua_setglobal(L, "Context");
    return NU_SUCCESS;
}