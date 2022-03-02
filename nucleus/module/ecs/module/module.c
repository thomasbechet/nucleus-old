/* Generated file: DO NOT EDIT ! */
#include <nucleus/module/ecs/module/module.h>

#include <nucleus/module/ecs/module/definition.h>
#include <nucleus/module/ecs/plugin/plugin.h>
#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/scene_manager.h>
#include <nucleus/module/ecs/plugin/component_manager.h>

static const uint32_t interface_count = 4;
static const char *interfaces[] = {
    NU_PLUGIN_INTERFACE_NAME, 
    NUECS_MANAGER_INTERFACE_NAME, 
    NUECS_SCENE_INTERFACE_NAME, 
    NUECS_QUERY_INTERFACE_NAME
};

static const uint32_t plugin_count = 1;
static const char *plugins[] = {
    NUECS_SCENE_PLUGIN_NAME
};

static nu_result_t plugin_get_list(uint32_t *count, const char ***plugin_list)
{
    *count = plugin_count;
    *plugin_list = plugins;
    return NU_SUCCESS;
}
static nu_result_t plugin_get_callbacks(const char *name, nu_plugin_callbacks_t *callbacks)
{
    if (NU_MATCH(name, NUECS_SCENE_PLUGIN_NAME)) {
        callbacks->initialize = nuecs_scene_plugin_initialize;
        callbacks->terminate = nuecs_scene_plugin_terminate;
        callbacks->update = nuecs_scene_plugin_update;
        return NU_SUCCESS;
    }

    return NU_FAILURE;
}

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name = NUECS_MODULE_NAME;
    info->id = NUECS_MODULE_ID;
    info->flags = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_PLUGIN_INTERFACE_NAME)) {
        nu_plugin_interface_t *i = (nu_plugin_interface_t*)interface;

        i->get_callbacks = plugin_get_callbacks;
        i->get_list = plugin_get_list;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_MANAGER_INTERFACE_NAME)) {
        nuecs_manager_interface_t *i = (nuecs_manager_interface_t*)interface;
        
        i->register_component = nuecs_manager_register_component;
        i->create_scene = nuecs_manager_create_scene;
        i->destroy_scene = nuecs_manager_destroy_scene;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_SCENE_INTERFACE_NAME)) {
        nuecs_scene_interface_t *i = (nuecs_scene_interface_t*)interface;
        
        i->progress = nuecs_scene_progress;
        i->register_system = nuecs_scene_register_system;
        i->create_entity = nuecs_scene_create_entity;
        i->destroy_entity = nuecs_scene_destroy_entity;
        i->entity_add_component = nuecs_scene_entity_add_component;
        i->entity_remove_component = nuecs_scene_entity_remove_component;
        i->save_file = nuecs_scene_save_file;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_QUERY_INTERFACE_NAME)) {
        nuecs_query_interface_t *i = (nuecs_query_interface_t*)interface;
        
        i->create = nuecs_query_create;
        i->destroy = nuecs_query_destroy;
        i->resolve_chunks = nuecs_query_resolve_chunks;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}