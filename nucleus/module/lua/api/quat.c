#include <nucleus/module/lua/api/quat.h>

#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

static int QuatObj__index(lua_State *L)
{
    luaL_checkudata(L, -2, NULUA_QUAT_TYPENAME);
    nu_quatf_t *q = (nu_quatf_t*)lua_touserdata(L, -2);
    size_t len;
    const char *key = luaL_checklstring(L, -1, &len);
    if (len == 1) {
        switch (key[0]) {
            case 'x': lua_pushnumber(L, (*q)[0]); return 1;
            case 'y': lua_pushnumber(L, (*q)[1]); return 1;
            case 'z': lua_pushnumber(L, (*q)[2]); return 1;
            case 'w': lua_pushnumber(L, (*q)[3]); return 1;
            default: return 0;
        }
    }
    return 0;
}
static int QuatObj__newindex(lua_State *L)
{
    luaL_checkudata(L, -3, NULUA_QUAT_TYPENAME);
    nu_quatf_t *q = (nu_quatf_t*)lua_touserdata(L, -3);
    size_t len;
    const char *key = luaL_checklstring(L, -2, &len);
    lua_Number value = luaL_checknumber(L, -1);
    if (len == 1) {
        switch (key[0]) {
            case 'x': (*q)[0] = value; return 0;
            case 'y': (*q)[1] = value; return 0;
            case 'z': (*q)[2] = value; return 0;
            case 'w': (*q)[3] = value; return 0;
            default: luaL_error(L, "unknown Quat index");
        }
    }
    luaL_error(L, "unknown Quat index");
    return 0;
}
static int QuatObj__tostring(lua_State *L)
{   
    luaL_checkudata(L, 1, NULUA_QUAT_TYPENAME);
    nu_quatf_t *q = (nu_quatf_t*)lua_touserdata(L, 1);
    lua_pushfstring(L, "quat(%f, %f, %f, %f)", (*q)[0], (*q)[1], (*q)[2], (*q)[3]);
    return 1;
}
static int QuatObj__mul(lua_State *L)
{
    nu_quatf_t *q = (nu_quatf_t*)lua_newuserdata(L, sizeof(nu_quatf_t));
    luaL_setmetatable(L, NULUA_QUAT_TYPENAME);
    nu_quatf_t *q1 = luaL_checkudata(L, 1, NULUA_QUAT_TYPENAME);
    nu_quatf_t *q2 = luaL_testudata(L, 2, NULUA_QUAT_TYPENAME);
    if (q2) {
        nu_quatf_mul(*q1, *q2, *q);
    } else {
        nu_vec3f_t *v2 = luaL_checkudata(L, 2, NULUA_VEC3_TYPENAME);
        nu_quatf_mulv(*q1, *v2, *q);
    }
    return 1;
}
int Quat_new_0(lua_State *L)
{
    nu_quatf_t *q = (nu_quatf_t*)lua_newuserdata(L, sizeof(nu_quatf_t));
    luaL_setmetatable(L, NULUA_QUAT_TYPENAME);
    nu_quatf_identity(*q);
    return 1;
}
static int Quat_new_2(lua_State *L)
{
    lua_Number angle = luaL_checknumber(L, -2);
    nu_vec3f_t *axis = (nu_vec3f_t*)luaL_checkudata(L, -1, NULUA_VEC3_TYPENAME);
    nu_quatf_t *q = (nu_quatf_t*)lua_newuserdata(L, sizeof(nu_quatf_t));
    luaL_setmetatable(L, NULUA_QUAT_TYPENAME);
    nu_quatf_from_axis(angle, *axis, *q);
    return 1;
}
static int Quat__call(lua_State *L)
{
    int argc = lua_gettop(L) - 1;
    switch (argc) {
        case 0: return Quat_new_0(L);
        case 2: return Quat_new_2(L);
        default: return Quat_new_0(L);
    }
}

nu_result_t nulua_register_quat_api(lua_State *L)
{
    static const luaL_Reg QuatObj_metatable[] = {
        {"__index",    QuatObj__index},
        {"__newindex", QuatObj__newindex},
        {"__tostring", QuatObj__tostring},
        {"__mul",      QuatObj__mul},
        {NULL, NULL}
    };
    static const luaL_Reg Quat_methods[] = {
        {NULL, NULL}
    };
    static const luaL_Reg Quat_metatable[] = {
        {"__call", Quat__call},
        {NULL, NULL}
    };

    /* create Quat */
    luaL_newlib(L, Quat_methods);
    luaL_newlib(L, Quat_metatable);
    lua_setmetatable(L, -2);
    lua_setglobal(L, NULUA_QUAT_TYPENAME);

    /* object metatable */
    luaL_newmetatable(L, NULUA_QUAT_TYPENAME);
    luaL_setfuncs(L, QuatObj_metatable, 0);
    lua_pop(L, 1);

    return NU_SUCCESS;
}