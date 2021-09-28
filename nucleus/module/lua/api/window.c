#include <nucleus/module/lua/api/window.h>

#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

static int Window_set_mode(lua_State *L)
{
    nu_window_mode_t mode = (nu_window_mode_t)luaL_checkinteger(L, 1); 
    nu_window_set_mode(mode);
    return 0;
}
static int Window_get_size(lua_State *L)
{
    nu_vec2u_t size;
    nu_window_get_size(size);
    lua_pushinteger(L, size[0]);
    lua_pushinteger(L, size[1]);
    return 2;
}
static int Window_set_size(lua_State *L)
{
    nu_vec2u_t size = {
        luaL_checkinteger(L, 1),
        luaL_checkinteger(L, 2)
    };
    nu_window_set_size(size);
    return 0;
}
static int Window_set_title(lua_State *L)
{
    nu_window_set_title(luaL_checkstring(L, 1));
    return 0;
}

nu_result_t nulua_register_window_api(lua_State *L)
{
    static const struct luaL_Reg Window_methods[] = {
        {"set_mode",  Window_set_mode},
        {"get_size",  Window_get_size},
        {"set_size",  Window_set_size},
        {"set_title", Window_set_title},
        {NULL, NULL}
    };

    luaL_newlib(L, Window_methods);

    lua_pushinteger(L, NU_WINDOW_MODE_FULLSCREEN); lua_setfield(L, -2, "FULLSCREEN");
    lua_pushinteger(L, NU_WINDOW_MODE_WINDOWED);   lua_setfield(L, -2, "WINDOWED");
    lua_pushinteger(L, NU_WINDOW_MODE_BORDERLESS); lua_setfield(L, -2, "BORDERLESS");

    lua_setglobal(L, "Window");

    return NU_SUCCESS;
}