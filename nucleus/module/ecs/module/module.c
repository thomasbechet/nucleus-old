#include <nucleus/module/ecs/module/module.h>

#include <nucleus/module/ecs/plugin/plugin.h>

static const uint32_t interface_count = 2;
static const char *interfaces[] = {
    NU_PLUGIN_INTERFACE_NAME,
    NUECS_PLUGIN_INTERFACE_NAME
};

static const uint32_t plugin_count = 1;
static const char *plugins[] = {
    NUECS_PLUGIN_NAME
};

static nu_result_t nuecs_plugin_get_list(uint32_t *count, const char ***plugin_list)
{
    *count = plugin_count;
    *plugin_list = plugins;
    return NU_SUCCESS;
}
static nu_result_t nuecs_plugin_get_callbacks(const char *name, nu_plugin_callbacks_t *callbacks)
{
    if (NU_MATCH(name, NUECS_PLUGIN_NAME)) {
        callbacks->initialize = nuecs_plugin_initialize;
        callbacks->terminate  = nuecs_plugin_terminate;
        callbacks->update     = nuecs_plugin_update;
        return NU_SUCCESS;
    }

    return NU_FAILURE;
}

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name            = NUECS_MODULE_NAME;
    info->id              = NUECS_MODULE_ID;
    info->flags           = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces      = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_PLUGIN_INTERFACE_NAME)) {
        nu_plugin_interface_t *i = (nu_plugin_interface_t*)interface;

        i->get_callbacks      = nuecs_plugin_get_callbacks;
        i->get_list           = nuecs_plugin_get_list;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_PLUGIN_INTERFACE_NAME)) {
        nuecs_plugin_interface_t *i = (nuecs_plugin_interface_t*)interface;

        i->world_create            = nuecs_world_create;
        i->world_progress          = nuecs_world_progress;
        i->component_register      = nuecs_component_register;
        i->system_register         = nuecs_system_register;
        i->entity_create           = nuecs_entity_create;
        i->entity_add_component    = nuecs_entity_add_component;
        i->entity_remove_component = nuecs_entity_remove_component;
        i->entity_destroy          = nuecs_entity_destroy;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}