#include <nucleus/module/lua/api/json.h>

#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

static nu_result_t resolve_value(lua_State *L, nu_json_value_t value)
{
    nu_json_type_t type = nu_json_value_get_type(value);
    if (type == NU_JSON_TYPE_ARRAY) {
        
    } else if (type == NU_JSON_TYPE_BOOL) {
        bool b;
        nu_json_value_as_bool(value, &b);
        lua_pushboolean(L, b);
    } else if (type == NU_JSON_TYPE_NULL) {
        lua_pushnil(L);
    } else if (type == NU_JSON_TYPE_NUMBER) {
        float f;
        nu_json_value_as_float(value, &f);
        lua_pushnumber(L, f);
    } else if (type == NU_JSON_TYPE_OBJECT) {
        nu_json_object_t object;
        nu_json_value_as_object(value, &object);
        lua_newtable(L);
        nu_json_object_iterator_t it = NU_NULL_HANDLE;
        while (nu_json_object_next(object, &it)) {
            resolve_value(L, nu_json_object_iterator_get_value(it));
            lua_setfield(L, -2, nu_json_object_iterator_get_name(it));
        }
    } else if (type == NU_JSON_TYPE_STRING) {
        const char *str;
        nu_json_value_as_cstr(value, &str);
        lua_pushstring(L, str);
    }

    return NU_SUCCESS;
}

int Json_parse(lua_State *L)
{
    const char *filename = luaL_checkstring(L, 1);

    /* resolve path */
    nu_string_t path;
    nu_string_allocate_cstr(&path, filename);
    nu_string_resolve_path(&path);

    /* load file */
    nu_json_t json;
    nu_result_t result = nu_json_allocate_from_file(&json, nu_string_get_cstr(path));
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NU_LOGGER_NAME, "Failed to parse json: %s", nu_string_get_cstr(path));

    /* build object */
    result = resolve_value(L, nu_json_get_root(json));
    NU_CHECK(result == NU_SUCCESS, goto cleanup1, NU_LOGGER_NAME, "Failed to resolve json.");
        
    /* free resources */
    nu_json_free(json);
    nu_string_free(path);
    return 1;

cleanup1:
    nu_json_free(json);
cleanup0:
    nu_string_free(path);
    lua_pushnil(L);
    return 1;
}

nu_result_t nulua_register_json_api(lua_State *L)
{
    static const luaL_Reg Json_methods[] = {
        {"parse", Json_parse},
        {NULL, NULL}
    };

    /* create Json */
    luaL_newlib(L, Json_methods);
    lua_setglobal(L, NULUA_JSON_TYPENAME);

    return NU_SUCCESS;
}