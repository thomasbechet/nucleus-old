#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

static int Vec3Obj__index(lua_State *L)
{
    luaL_checkudata(L, -2, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_touserdata(L, -2);
    size_t len;
    const char *key = luaL_checklstring(L, -1, &len);
    if (len == 1) {
        switch (key[0]) {
            case 'x': lua_pushnumber(L, (*v)[0]); return 1;
            case 'y': lua_pushnumber(L, (*v)[1]); return 1;
            case 'z': lua_pushnumber(L, (*v)[2]); return 1;
            default: return 0;
        }
    }
    return 0;
}
static int Vec3Obj__newindex(lua_State *L)
{
    luaL_checkudata(L, -3, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_touserdata(L, -3);
    size_t len;
    const char *key = luaL_checklstring(L, -2, &len);
    lua_Number value = luaL_checknumber(L, -1);
    if (len == 1) {
        switch (key[0]) {
            case 'x': (*v)[0] = value; return 0;
            case 'y': (*v)[1] = value; return 0;
            case 'z': (*v)[2] = value; return 0;
            default: luaL_error(L, "unknown Vec3 index");
        }
    }
    luaL_error(L, "unknown Vec3 index");
    return 0;
}
static int Vec3Obj__tostring(lua_State *L)
{   
    luaL_checkudata(L, -1, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_touserdata(L, -1);
    lua_pushfstring(L, "vec(%f, %f, %f)", (*v)[0], (*v)[1], (*v)[2]);
    return 1;
}
static int Vec3Obj__add(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC3_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC3_TYPENAME))) {
            nu_vec3f_add(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec3f_adds(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec3f_t*)luaL_checkudata(L, 2, NULUA_VEC3_TYPENAME);
        nu_vec3f_adds(*v2, s, *v);
    }
    return 1;
}
static int Vec3Obj__sub(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC3_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC3_TYPENAME))) {
            nu_vec3f_sub(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec3f_subs(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec3f_t*)luaL_checkudata(L, 2, NULUA_VEC3_TYPENAME);
        nu_vec3f_sub((const nu_vec3f_t){s, s, s}, *v2, *v);
    }
    return 1;
}
static int Vec3Obj__mul(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC3_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC3_TYPENAME))) {
            nu_vec3f_mul(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec3f_muls(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec3f_t*)luaL_checkudata(L, 2, NULUA_VEC3_TYPENAME);
        nu_vec3f_muls(*v2, s, *v);
    }
    return 1;
}
static int Vec3Obj__div(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC3_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC3_TYPENAME))) {
            nu_vec3f_div(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec3f_divs(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec3f_t*)luaL_checkudata(L, 2, NULUA_VEC3_TYPENAME);
        nu_vec3f_div((const nu_vec3f_t){s, s, s}, *v2, *v);
    }
    return 1;
}
static int Vec3Obj__unm(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v1 = (nu_vec3f_t*)lua_touserdata(L, 1);
    nu_vec3f_muls(*v1, -1.0f, *v);
    return 1;
}
static int Vec3Obj__len(lua_State *L)
{
    nu_vec3f_t *v1 = (nu_vec3f_t*)lua_touserdata(L, 1);
    lua_pushnumber(L, nu_vec3f_length(*v1));
    return 1;
}
int Vec3_new_0(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_zero(*v);
    return 1;
}
static int Vec3_new_1(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v1;
    if ((v1 = luaL_testudata(L, -2, NULUA_VEC3_TYPENAME))) {
        nu_vec3f_copy(*v1, *v);
    } else {
        lua_Number n = luaL_checknumber(L, -2);
        nu_vec3f_copy((nu_vec3f_t){n, n, n}, *v);
    }
    return 1;
}
static int Vec3_new_3(lua_State *L)
{
    nu_vec3f_t *v = (nu_vec3f_t*)lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    (*v)[0] = luaL_checknumber(L, -4);
    (*v)[1] = luaL_checknumber(L, -3);
    (*v)[2] = luaL_checknumber(L, -2);
    return 1;
}
static int Vec3__call(lua_State *L)
{
    int argc = lua_gettop(L) - 1;
    switch (argc) {
        case 0: return Vec3_new_0(L);
        case 1: return Vec3_new_1(L);
        case 3: return Vec3_new_3(L);
        default: return Vec3_new_0(L);
    }
}
static int Vec3_normalize(lua_State *L)
{
    luaL_checkudata(L, -1, NULUA_VEC3_TYPENAME);
    nu_vec3f_t *v = (nu_vec3f_t*)lua_touserdata(L, -1);
    nu_vec3f_normalize(*v);
    return 0;
}
static int Vec3_length(lua_State *L)
{
    return Vec3Obj__len(L);
}

static int Vec4Obj__index(lua_State *L)
{
    luaL_checkudata(L, -2, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v = (nu_vec4f_t*)lua_touserdata(L, -2);
    size_t len;
    const char *key = luaL_checklstring(L, -1, &len);
    if (len == 1) {
        switch (key[0]) {
            case 'x': lua_pushnumber(L, (*v)[0]); return 1;
            case 'y': lua_pushnumber(L, (*v)[1]); return 1;
            case 'z': lua_pushnumber(L, (*v)[2]); return 1;
            case 'w': lua_pushnumber(L, (*v)[3]); return 1;
            default: return 0;
        }
    }
    return 0;
}
static int Vec4Obj__newindex(lua_State *L)
{
    luaL_checkudata(L, -3, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v = (nu_vec4f_t*)lua_touserdata(L, -3);
    size_t len;
    const char *key = luaL_checklstring(L, -2, &len);
    lua_Number value = luaL_checknumber(L, -1);
    if (len == 1) {
        switch (key[0]) {
            case 'x': (*v)[0] = value; return 0;
            case 'y': (*v)[1] = value; return 0;
            case 'z': (*v)[2] = value; return 0;
            case 'w': (*v)[3] = value; return 0;
            default: luaL_error(L, "unknown Vec4 index");
        }
    }
    luaL_error(L, "unknown Vec4 index");
    return 0;
}
static int Vec4Obj__tostring(lua_State *L)
{   
    luaL_checkudata(L, -1, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v = (nu_vec4f_t*)lua_touserdata(L, -1);
    lua_pushfstring(L, "vec(%f, %f, %f, %f)", (*v)[0], (*v)[1], (*v)[2], (*v)[3]);
    return 1;
}
static int Vec4Obj__add(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC4_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC4_TYPENAME))) {
            nu_vec4f_add(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec4f_adds(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec4f_t*)luaL_checkudata(L, 2, NULUA_VEC4_TYPENAME);
        nu_vec4f_adds(*v2, s, *v);
    }
    return 1;
}
static int Vec4Obj__sub(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC4_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC4_TYPENAME))) {
            nu_vec4f_sub(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec4f_subs(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec4f_t*)luaL_checkudata(L, 2, NULUA_VEC4_TYPENAME);
        nu_vec4f_sub((const nu_vec4f_t){s, s, s, s}, *v2, *v);
    }
    return 1;
}
static int Vec4Obj__mul(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC4_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC4_TYPENAME))) {
            nu_vec4f_mul(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec4f_muls(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec4f_t*)luaL_checkudata(L, 2, NULUA_VEC4_TYPENAME);
        nu_vec4f_muls(*v2, s, *v);
    }
    return 1;
}
static int Vec4Obj__div(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v1, *v2;
    if ((v1 = luaL_testudata(L, 1, NULUA_VEC4_TYPENAME))) {
        if ((v2 = luaL_testudata(L, 2, NULUA_VEC4_TYPENAME))) {
            nu_vec4f_div(*v1, *v2, *v);
        } else {
            lua_Number s = luaL_checknumber(L, 2);
            nu_vec4f_divs(*v1, s, *v);
        }
    } else {
        lua_Number s = luaL_checknumber(L, 1);
        v2 = (nu_vec4f_t*)luaL_checkudata(L, 2, NULUA_VEC4_TYPENAME);
        nu_vec4f_div((const nu_vec4f_t){s, s, s, s}, *v2, *v);
    }
    return 1;
}
static int Vec4Obj__unm(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v1 = (nu_vec4f_t*)lua_touserdata(L, 1);
    nu_vec4f_muls(*v1, -1.0f, *v);
    return 1;
}
int Vec4_new_0(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    nu_vec4f_zero(*v);
    return 1;
}
static int Vec4_new_1(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    if (lua_type(L, -2) == LUA_TNUMBER) {
        lua_Number n = lua_tonumber(L, -2);
        nu_vec4f_copy((nu_vec4f_t){n, n, n, n}, *v);
    } else {
        nu_vec3f_t *v3;
        nu_vec4f_t *v4;
        if ((v3 = luaL_testudata(L, -2, NULUA_VEC3_TYPENAME))) {
            nu_vec4f_from_v3(*v3, 0.0f, *v);
        } else if ((v4 = luaL_testudata(L, -2, NULUA_VEC4_TYPENAME))) {
            nu_vec4f_copy(*v4, *v);
        }
    }
    return 1;
}
static int Vec4_new_4(lua_State *L)
{
    nu_vec4f_t *v = (nu_vec4f_t*)lua_newuserdata(L, sizeof(nu_vec4f_t));
    luaL_setmetatable(L, NULUA_VEC4_TYPENAME);
    (*v)[0] = luaL_checknumber(L, -5);
    (*v)[1] = luaL_checknumber(L, -4);
    (*v)[2] = luaL_checknumber(L, -3);
    (*v)[3] = luaL_checknumber(L, -2);
    return 1;
}
static int Vec4__call(lua_State *L)
{
    int argc = lua_gettop(L) - 1;
    switch (argc) {
        case 0: return Vec4_new_0(L);
        case 1: return Vec4_new_1(L);
        case 4: return Vec4_new_4(L);
        default: return Vec4_new_0(L);
    }
}
static int Vec4_normalize(lua_State *L)
{
    luaL_checkudata(L, -1, NULUA_VEC4_TYPENAME);
    nu_vec4f_t *v = (nu_vec4f_t*)lua_touserdata(L, -1);
    nu_vec4f_normalize(*v);
    return 0;
}

static void nulua_register_vec_constants(lua_State *L)
{
    lua_getglobal(L, NULUA_VEC3_TYPENAME);

    nu_vec3f_t *up = lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_copy(NU_VEC3F_UP, *up);
    lua_setfield(L, -2, "UP");

    nu_vec3f_t *down = lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_copy(NU_VEC3F_DOWN, *down);
    lua_setfield(L, -2, "DOWN");

    nu_vec3f_t *left = lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_copy(NU_VEC3F_LEFT, *left);
    lua_setfield(L, -2, "LEFT");

    nu_vec3f_t *right = lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_copy(NU_VEC3F_RIGHT, *right);
    lua_setfield(L, -2, "RIGHT");

    nu_vec3f_t *forward = lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_copy(NU_VEC3F_FORWARD, *forward);
    lua_setfield(L, -2, "FORWARD");

    nu_vec3f_t *backward = lua_newuserdata(L, sizeof(nu_vec3f_t));
    luaL_setmetatable(L, NULUA_VEC3_TYPENAME);
    nu_vec3f_copy(NU_VEC3F_BACKWARD, *backward);
    lua_setfield(L, -2, "BACKWARD");

    lua_pop(L, 1);
}

nu_result_t nulua_register_vec_api(lua_State *L)
{
    static const luaL_Reg Vec3Obj_metatable[] = {
        {"__index",    Vec3Obj__index},
        {"__newindex", Vec3Obj__newindex},
        {"__tostring", Vec3Obj__tostring},
        {"__add",      Vec3Obj__add},
        {"__sub",      Vec3Obj__sub},
        {"__mul",      Vec3Obj__mul},
        {"__div",      Vec3Obj__div},
        {"__unm",      Vec3Obj__unm},
        {"__len",      Vec3Obj__len},
        {NULL, NULL}
    };
    static const luaL_Reg Vec3_methods[] = {
        {"normalize", Vec3_normalize},
        {"length",    Vec3_length},
        {NULL, NULL}
    };
    static const luaL_Reg Vec3_metatable[] = {
        {"__call", Vec3__call},
        {NULL, NULL}
    };

    /* create Vec3 */
    luaL_newlib(L, Vec3_methods);
    luaL_newlib(L, Vec3_metatable);
    lua_setmetatable(L, -2);
    lua_setglobal(L, NULUA_VEC3_TYPENAME);

    /* object metatable */
    luaL_newmetatable(L, NULUA_VEC3_TYPENAME);
    luaL_setfuncs(L, Vec3Obj_metatable, 0);
    lua_pop(L, 1);

    nulua_register_vec_constants(L);

    static const luaL_Reg Vec4Obj_metatable[] = {
        {"__index",    Vec4Obj__index},
        {"__newindex", Vec4Obj__newindex},
        {"__tostring", Vec4Obj__tostring},
        {"__add",      Vec4Obj__add},
        {"__sub",      Vec4Obj__sub},
        {"__mul",      Vec4Obj__mul},
        {"__div",      Vec4Obj__div},
        {"__unm",      Vec4Obj__unm},
        {NULL, NULL}
    };
    static const luaL_Reg Vec4_methods[] = {
        {"normalize", Vec4_normalize},
        {NULL, NULL}
    };
    static const luaL_Reg Vec4_metatable[] = {
        {"__call", Vec4__call},
        {NULL, NULL}
    };

    /* create Vec4 */
    luaL_newlib(L, Vec4_methods);
    luaL_newlib(L, Vec4_metatable);
    lua_setmetatable(L, -2);
    lua_setglobal(L, NULUA_VEC4_TYPENAME);

    /* object metatable */
    luaL_newmetatable(L, NULUA_VEC4_TYPENAME);
    luaL_setfuncs(L, Vec4Obj_metatable, 0);
    lua_pop(L, 1);

    return NU_SUCCESS;
}