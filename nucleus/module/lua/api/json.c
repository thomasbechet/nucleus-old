#include <nucleus/module/lua/api/json.h>

#include <nucleus/module/lua/api/vec.h>

#include <lauxlib.h>

static void resolve_value(lua_State *L, nu_json_value_t value)
{
    nu_json_type_t type = nu_json_value_get_type(value);
    if (type == NU_JSON_TYPE_ARRAY) {

    } else if (type == NU_JSON_TYPE_BOOL) {

    } else if (type == NU_JSON_TYPE_NULL) {

    } else if (type == NU_JSON_TYPE_NUMBER) {

    } else if (type == NU_JSON_TYPE_OBJECT) {

    } else if (type == NU_JSON_TYPE_STRING) {
        
    }
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
    
    
    nu_json_free(json);
    nu_string_free(path);

    lua_pushnumber(L, 3.0);
    return 1;

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