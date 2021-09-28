#include <nucleus/module/lua/api/renderer.h>

#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

static int Renderer_camera_set_view(lua_State *L)
{
    nu_vec3f_t *eye     = (nu_vec3f_t*)luaL_checkudata(L, 1, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *forward = (nu_vec3f_t*)luaL_checkudata(L, 2, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *up      = (nu_vec3f_t*)luaL_checkudata(L, 3, NULUA_VEC3_TYPENAME);
    nu_renderer_camera_set_view(NU_NULL_HANDLE, *eye, *forward, *up);
    return 0;
}
static int Renderer_camera_set_fov(lua_State *L)
{
    lua_Number fov = luaL_checknumber(L, 1);
    nu_renderer_camera_set_fov(NU_NULL_HANDLE, fov);
    return 0;
}
static int Renderer_viewport_set_size(lua_State *L)
{
    nu_vec2u_t size = {
        luaL_checkinteger(L, 1),
        luaL_checkinteger(L, 2)
    };
    nu_renderer_viewport_set_size(size);
    return 0;
}

nu_result_t nulua_register_renderer_api(lua_State *L)
{
    static const struct luaL_Reg Renderer_methods[] = {
        {"camera_set_view",   Renderer_camera_set_view},
        {"camera_set_fov",    Renderer_camera_set_fov},
        {"viewport_set_size", Renderer_viewport_set_size},
        {NULL, NULL}
    };

    luaL_newlib(L, Renderer_methods);
    lua_setglobal(L, "Renderer");
    return NU_SUCCESS;
}