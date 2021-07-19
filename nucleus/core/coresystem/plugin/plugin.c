#include <nucleus/core/coresystem/plugin/plugin.h>

#include <nucleus/core/coresystem/module/interface.h>
#include <nucleus/core/coresystem/logger/logger.h>

#define MAX_PLUGIN_COUNT 32
#define NU_MODULE_GET_PLUGIN_NAME "nu_module_get_plugin"

typedef nu_result_t (*nu_module_plugin_loader_pfn_t)(const char*, nu_plugin_interface_t*);

typedef struct {
    nu_module_t module;
    nu_plugin_interface_t interface;
    const char *name;
} nu_plugin_t;

typedef struct {
    nu_plugin_t plugins[MAX_PLUGIN_COUNT];
    uint32_t plugin_count;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_plugin_initialize(void)
{
    /* initialize data */
    for (uint32_t i = 0; i < MAX_PLUGIN_COUNT; i++) {
        _system.plugins[i].module = NULL;
    }
    _system.plugin_count = 0;

    return NU_SUCCESS;
}
nu_result_t nu_plugin_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_plugin_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_plugin_stop(void)
{
    for (uint32_t i = 0; i < _system.plugin_count; i++) {
        if (_system.plugins[i].interface.terminate) {
            _system.plugins[i].interface.terminate();
        }
    }

    return NU_SUCCESS;
}
nu_result_t nu_plugin_update(void)
{
    for (uint32_t i = 0; i < _system.plugin_count; i++) {
        if (_system.plugins[i].interface.update) {
            _system.plugins[i].interface.update();
        }
    }

    return NU_SUCCESS;
}
nu_result_t nu_plugin_fixed_update(void)
{
    for (uint32_t i = 0; i < _system.plugin_count; i++) {
        if (_system.plugins[i].interface.fixed_update) {
            _system.plugins[i].interface.fixed_update();
        }
    }

    return NU_SUCCESS;
}
nu_result_t nu_plugin_late_update(void)
{
    for (uint32_t i = 0; i < _system.plugin_count; i++) {
        if (_system.plugins[i].interface.late_update) {
            _system.plugins[i].interface.late_update();
        }
    }

    return NU_SUCCESS;
}

nu_result_t nu_plugin_require(nu_module_t module, const char *plugin)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* check exists */
    for (uint32_t i = 0; i < _system.plugin_count; i++) {
        if (_system.plugins[i].module == module && NU_MATCH(_system.plugins[i].name, plugin)) {
            return NU_SUCCESS;
        }
    }

    /* error check */
    if (_system.plugin_count >= MAX_PLUGIN_COUNT) return NU_FAILURE;

    /* choose id */
    uint32_t id = _system.plugin_count;

    /* load plugin loader interface */
    nu_module_plugin_loader_pfn_t plugin_loader;
    result = nu_module_load_function(module, NU_MODULE_GET_PLUGIN_NAME, (nu_pfn_t*)&plugin_loader);
    if (result != NU_SUCCESS) {
        nu_core_log(NU_WARNING, "Failed to load '%s' function for plugin: %s.\n", NU_MODULE_GET_PLUGIN_NAME, plugin);
        return result;
    }

    /* load plugin interface */
    memset(&_system.plugins[id].interface, 0, sizeof(nu_plugin_interface_t));
    result = plugin_loader(plugin, &_system.plugins[id].interface);
    if (result != NU_SUCCESS) {
        nu_core_log(NU_WARNING, "Failed to load plugin interface: %s.\n", plugin);
        return result;
    }

    /* save plugin name and module*/
    _system.plugins[id].module = module;
    _system.plugins[id].name   = plugin;

    /* initialize plugin */
    if (_system.plugins[id].interface.initialize) {
        result = _system.plugins[id].interface.initialize();
        if (result != NU_SUCCESS) {
            nu_core_log(NU_WARNING, "Failed to initialize plugin: %s.\n", plugin);
            return result;
        }
    }

    /* increment id */
    _system.plugin_count++;

    return NU_SUCCESS;
}