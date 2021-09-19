#include <nucleus/module/lua/plugin/plugin.h>

#include <nucleus/module/lua/common/logger.h>
#include <nucleus/module/lua/common/allocator.h>
#include <nucleus/module/lua/api/api.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct {
    lua_Integer table_ref;
    lua_Integer update_ref;
    lua_Integer start_ref;
    lua_Integer stop_ref;
    bool started;
} nulua_plugin_data_t;

typedef struct {
    lua_State *L;
    nu_indexed_array_t plugins;
} nulua_module_data_t;

static nulua_module_data_t _module;

nu_result_t nulua_plugin_initialize(void)
{
    /* initialize resources */
    memset(&_module, 0, sizeof(nulua_module_data_t));
    nu_indexed_array_allocate(&_module.plugins, sizeof(nulua_plugin_data_t));

    /* create state */
    _module.L = lua_newstate(nulua_allocator, NULL);
    NU_CHECK(_module.L, return NU_FAILURE, NULUA_LOGGER_NAME, "Failed to create lua state.");

    /* add base libs */
    luaL_openlibs(_module.L);

    /* register api */
    nu_result_t result = nulua_register_api(_module.L);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NULUA_LOGGER_NAME, "Failed to register api.");

    return NU_SUCCESS;

cleanup0:
    lua_close(_module.L);
    return NU_FAILURE;
}
nu_result_t nulua_plugin_terminate(void)
{
    /* stop plugins */
    uint32_t plugin_count = nu_indexed_array_get_size(_module.plugins);
    nulua_plugin_data_t *plugins = (nulua_plugin_data_t*)nu_indexed_array_get_data(_module.plugins);
    for (uint32_t i = 0; i < plugin_count; i++) {
        if (plugins[i].started) {
            if (plugins[i].stop_ref != LUA_REFNIL) {
                lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].stop_ref);
                lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].table_ref);
                int error = lua_pcall(_module.L, 1, 0, 0);
                NU_CHECK(!error, NU_NOOP, NULUA_LOGGER_NAME, "Failed to stop plugin: %s.", lua_tostring(_module.L, -1));
            }
            plugins[i].started = false;
        }
    }

    /* free resources */
    lua_close(_module.L);
    nu_indexed_array_free(_module.plugins);
    return NU_SUCCESS;
}
nu_result_t nulua_plugin_update(void)
{
    /* update plugins */
    uint32_t plugin_count = nu_indexed_array_get_size(_module.plugins);
    nulua_plugin_data_t *plugins = (nulua_plugin_data_t*)nu_indexed_array_get_data(_module.plugins);
    for (uint32_t i = 0; i < plugin_count; i++) {
        if (!plugins[i].started) { /* start the plugin */
            if (plugins[i].start_ref != LUA_REFNIL) {
                lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].start_ref);
                lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].table_ref);
                int error = lua_pcall(_module.L, 1, 0, 0);
                NU_CHECK(!error, NU_NOOP, NULUA_LOGGER_NAME, "Failed to start plugin: %s.", lua_tostring(_module.L, -1));
            }
            plugins[i].started = true;
        } else if (plugins[i].update_ref != LUA_REFNIL) { /* update plugin */
            lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].update_ref);
            lua_rawgeti(_module.L, LUA_REGISTRYINDEX, plugins[i].table_ref);
            int error = lua_pcall(_module.L, 1, 0, 0);
            NU_CHECK(!error, NU_NOOP, NULUA_LOGGER_NAME, "Failed to update plugin: %s.", lua_tostring(_module.L, -1));
        }
    }

    return NU_SUCCESS;
}

nu_result_t nulua_plugin_load(const char *filename, nulua_plugin_t *handle)
{
    nu_string_t path;
    nu_string_allocate_cstr(&path, filename);
    nu_string_resolve_path(&path);
    NU_CHECK(nu_string_is_filename(path), goto cleanup0, NULUA_LOGGER_NAME, "'%s' is not a valid script.");
    
    /* load chunk */
    int error = luaL_loadfile(_module.L, nu_string_get_cstr(path));
    NU_CHECK(!error, goto cleanup0, NULUA_LOGGER_NAME, "Failed to load script '%s': %s.", 
        nu_string_get_cstr(path), lua_tostring(_module.L, -1));
    
    /* call chunk */
    error = lua_pcall(_module.L, 0, 1, 0);
    NU_CHECK(!error, goto cleanup0, NULUA_LOGGER_NAME, "Failed to execute script '%s': %s.", 
        nu_string_get_cstr(path), lua_tostring(_module.L, -1));
    
    /* check return value */
    NU_CHECK(lua_istable(_module.L, -1), goto cleanup0, NULUA_LOGGER_NAME, "Return value is not a table (expecting a table).");

    /* recover references */
    nulua_plugin_data_t data;

    int type = lua_getfield(_module.L, -1, "on_start");
    NU_CHECK(type == LUA_TFUNCTION || type != LUA_TNIL, goto cleanup0, NULUA_LOGGER_NAME, "'on_start' is not a function.");
    type = lua_getfield(_module.L, -2, "on_stop");
    NU_CHECK(type != LUA_TFUNCTION || type != LUA_TNIL, goto cleanup0, NULUA_LOGGER_NAME, "'on_stop' is not a function.");
    type = lua_getfield(_module.L, -3, "on_update");
    NU_CHECK(type != LUA_TFUNCTION || type != LUA_TNIL, goto cleanup0, NULUA_LOGGER_NAME, "'on_update' is not a function.");

    data.update_ref = luaL_ref(_module.L, LUA_REGISTRYINDEX);
    data.stop_ref   = luaL_ref(_module.L, LUA_REGISTRYINDEX);
    data.start_ref  = luaL_ref(_module.L, LUA_REGISTRYINDEX);
    data.table_ref  = luaL_ref(_module.L, LUA_REGISTRYINDEX);
    data.started    = false;

    /* return handle */
    uint32_t id;
    nu_indexed_array_add(_module.plugins, &data, &id);
    NU_HANDLE_SET_ID(*handle, id);

    /* free resources */
    nu_string_free(path);
    return NU_SUCCESS;

cleanup0:
    nu_string_free(path);
    return NU_FAILURE;
}