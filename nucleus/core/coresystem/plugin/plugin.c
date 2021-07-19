#include <nucleus/core/coresystem/plugin/plugin.h>

#include <nucleus/core/coresystem/module/interface.h>
#include <nucleus/core/coresystem/logger/logger.h>
#include <nucleus/core/utils/array.h>
#include <nucleus/core/utils/string.h>

#define NU_MODULE_GET_PLUGIN_NAME "nu_module_get_plugin"

typedef nu_result_t (*nu_module_plugin_loader_pfn_t)(const char*, nu_plugin_interface_t*);

typedef struct {
    nu_module_t module;
    nu_plugin_interface_t interface;
    nu_string_t name;
} nu_plugin_data_t;

typedef struct {
    nu_array_t plugins;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_plugin_initialize(void)
{
    /* allocate memory */
    nu_array_allocate(sizeof(nu_plugin_data_t), &_system.plugins);

    return NU_SUCCESS;
}
nu_result_t nu_plugin_terminate(void)
{
    /* free resources */
    uint32_t plugin_count = nu_array_get_size(_system.plugins);
    nu_plugin_data_t *plugins = (nu_plugin_data_t*)nu_array_get_data(_system.plugins);
    for (uint32_t i = 0; i < plugin_count; i++) {
        nu_string_free(plugins[i].name);
    }
    nu_array_free(_system.plugins);

    return NU_SUCCESS;
}
nu_result_t nu_plugin_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_plugin_stop(void)
{
    uint32_t size = nu_array_get_size(_system.plugins);
    nu_plugin_data_t *plugins = (nu_plugin_data_t*)nu_array_get_data(_system.plugins);
    for (uint32_t i = 0; i < size; i++) {
        if (plugins[i].interface.terminate) {
            plugins[i].interface.terminate();
        }
    }
    return NU_SUCCESS;
}
nu_result_t nu_plugin_update(void)
{
    uint32_t size = nu_array_get_size(_system.plugins);
    nu_plugin_data_t *plugins = (nu_plugin_data_t*)nu_array_get_data(_system.plugins);
    for (uint32_t i = 0; i < size; i++) {
        if (plugins[i].interface.update) {
            plugins[i].interface.update();
        }
    }
    return NU_SUCCESS;
}
nu_result_t nu_plugin_fixed_update(void)
{
    uint32_t size = nu_array_get_size(_system.plugins);
    nu_plugin_data_t *plugins = (nu_plugin_data_t*)nu_array_get_data(_system.plugins);
    for (uint32_t i = 0; i < size; i++) {
        if (plugins[i].interface.fixed_update) {
            plugins[i].interface.fixed_update();
        }
    }
    return NU_SUCCESS;
}
nu_result_t nu_plugin_late_update(void)
{
    uint32_t size = nu_array_get_size(_system.plugins);
    nu_plugin_data_t *plugins = (nu_plugin_data_t*)nu_array_get_data(_system.plugins);
    for (uint32_t i = 0; i < size; i++) {
        if (plugins[i].interface.late_update) {
            plugins[i].interface.late_update();
        }
    }
    return NU_SUCCESS;
}

nu_result_t nu_plugin_require(nu_module_t module, const char *plugin_name)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* check exists */
    uint32_t size = nu_array_get_size(_system.plugins);
    nu_plugin_data_t *plugins = (nu_plugin_data_t*)nu_array_get_data(_system.plugins);
    for (uint32_t i = 0; i < size; i++) {
        if (plugins[i].module == module && NU_MATCH(nu_string_get_cstr(plugins[i].name), plugin_name)) {
            return NU_SUCCESS;
        }
    }

    /* create new plugin */
    nu_plugin_data_t plugin;
    memset(&plugin, 0, sizeof(nu_plugin_data_t));

    /* load plugin loader interface */
    nu_module_plugin_loader_pfn_t plugin_loader;
    result = nu_module_load_function(module, NU_MODULE_GET_PLUGIN_NAME, (nu_pfn_t*)&plugin_loader);
    if (result != NU_SUCCESS) {
        nu_core_log(NU_WARNING, "Failed to load '%s' function for plugin: %s.\n", NU_MODULE_GET_PLUGIN_NAME, plugin_name);
        return result;
    }

    /* load plugin interface */
    result = plugin_loader(plugin_name, &plugin.interface);
    if (result != NU_SUCCESS) {
        nu_core_log(NU_WARNING, "Failed to load plugin interface: %s.\n", plugin);
        return result;
    }

    /* save plugin name and module*/
    plugin.module = module;
    nu_string_allocate_from(plugin_name, &plugin.name);

    /* initialize plugin */
    if (plugin.interface.initialize) {
        result = plugin.interface.initialize();
        if (result != NU_SUCCESS) {
            nu_core_log(NU_WARNING, "Failed to initialize plugin: %s.\n", plugin_name);
            return result;
        }
    }

    /* add the plugin */
    nu_array_push(_system.plugins, &plugin);

    return NU_SUCCESS;
}