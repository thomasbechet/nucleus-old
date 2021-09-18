#include <nucleus/module/lua/plugin/plugin.h>

#include <nucleus/module/lua/common/logger.h>
#include <nucleus/module/lua/plugin/allocator.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct {
    lua_Integer table_ref;
    lua_Integer update_ref;
} nulua_plugin_data_t;

typedef struct {
    lua_State *L;
    nu_array_t plugins;
} nulua_module_data_t;

static nulua_module_data_t _module;

static nu_result_t nulua_stack_dump_types(void)
{
    nu_string_t dump;
    nu_string_allocate(&dump);
    uint32_t top = lua_gettop(_module.L);
    for (uint32_t i = 1; i <= top; i++) {
        nu_string_t type;
        int type_id = lua_type(_module.L, i);
        nu_string_allocate_format(&type, "[%ld] %s ", i, lua_typename(_module.L, type_id));
        nu_string_append(&dump, type);
        nu_string_free(type);
    }
    nu_info(NULUA_LOGGER_NAME, "STACK: %s", nu_string_get_cstr(dump));
    nu_string_free(dump);

    return NU_SUCCESS;
}

static nu_result_t nulua_load_plugin(const char *filename)
{
    nu_path_t path;
    nu_path_allocate_cstr(filename, &path);
    NU_CHECK(nu_path_is_filename(path), goto cleanup0, NULUA_LOGGER_NAME, "'%s' is not a valid script.");
    
    /* load chunk */
    int error = luaL_loadfile(_module.L, nu_path_get_cstr(path));
    NU_CHECK(!error, goto cleanup0, NULUA_LOGGER_NAME, "Failed to load script '%s': %s.", 
        nu_path_get_cstr(path), lua_tostring(_module.L, -1));
    
    /* call chunk */
    error = lua_pcall(_module.L, 0, 1, 0);
    NU_CHECK(!error, goto cleanup0, NULUA_LOGGER_NAME, "Failed to execute script '%s': %s.", 
        nu_path_get_cstr(path), lua_tostring(_module.L, -1));
    
    /* check return value */
    NU_CHECK(lua_istable(_module.L, -1), goto cleanup0, NULUA_LOGGER_NAME, "Return value is not a table.");

    /* recover references */
    nulua_plugin_data_t data;

    int type = lua_getfield(_module.L, -1, "on_update");
    NU_CHECK(type == LUA_TFUNCTION, goto cleanup0, NULUA_LOGGER_NAME, "'on_update' is not a function.");

    data.update_ref = luaL_ref(_module.L, LUA_REGISTRYINDEX);
    data.table_ref  = luaL_ref(_module.L, LUA_REGISTRYINDEX);
    nu_array_push(_module.plugins, &data);

    nu_path_free(path);
    return NU_SUCCESS;

cleanup0:
    nu_path_free(path);
    return NU_FAILURE;
}

nu_result_t nulua_plugin_initialize(void)
{
    /* initialize resources */
    memset(&_module, 0, sizeof(nulua_module_data_t));
    nu_array_allocate(&_module.plugins, sizeof(nulua_plugin_data_t));

    /* create state */
    _module.L = lua_newstate(nulua_allocator, NULL);
    NU_CHECK(_module.L, return NU_FAILURE, NULUA_LOGGER_NAME, "Failed to create lua state.");

    /* add base libs */
    luaL_openlibs(_module.L);

    /* load script */
    // nu_result_t result = nulua_load_plugin("$ENGINE_DIR/script/test.lua");
    // NU_CHECK(result == NU_SUCCESS, NU_NOOP, NULUA_LOGGER_NAME, "Failed to load script.");

    return NU_SUCCESS;
}
nu_result_t nulua_plugin_terminate(void)
{
    lua_close(_module.L);
    nu_array_free(_module.plugins);
    return NU_SUCCESS;
}
nu_result_t nulua_plugin_update(void)
{
    uint32_t plugin_count = nu_array_get_size(_module.plugins);
    const nulua_plugin_data_t *plugins = (const nulua_plugin_data_t *)nu_array_get_data_const(_module.plugins);
    for (uint32_t i = 0; i < plugin_count; i++) {
        lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].update_ref);
        lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].table_ref);
        int error = lua_pcall(_module.L, 1, 0, 0);
        NU_CHECK(!error, NU_NOOP, NULUA_LOGGER_NAME, "Failed to update plugin: %s.", lua_tostring(_module.L, -1));
    }

    return NU_SUCCESS;
}