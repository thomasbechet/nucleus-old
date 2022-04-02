/* Generated file: DO NOT EDIT ! */
#include <nucleus/module/ecs/module/module.h>

#include <nucleus/module/ecs/module/definition.h>
#include <nucleus/module/ecs/module/implementation.h>

static const uint32_t interface_count = 7;
static const char *interfaces[] = {
    NU_PLUGIN_INTERFACE_NAME, 
    NUECS_ARCHETYPE_INTERFACE_NAME, 
    NUECS_SCENE_INTERFACE_NAME, 
    NUECS_QUERY_INTERFACE_NAME, 
    NUECS_ENTITY_INTERFACE_NAME, 
    NUECS_COMPONENT_INTERFACE_NAME, 
    NUECS_SYSTEM_INTERFACE_NAME
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
        callbacks->initialize = nuecs_scene_plugin_initialize_impl;
        callbacks->terminate = nuecs_scene_plugin_terminate_impl;
        callbacks->update = nuecs_scene_plugin_update_impl;
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
    } else if (NU_MATCH(name, NUECS_ARCHETYPE_INTERFACE_NAME)) {
        nuecs_archetype_interface_t *i = (nuecs_archetype_interface_t*)interface;
        
        i->debug_archetypes = nuecs_archetype_debug_archetypes_impl;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_SCENE_INTERFACE_NAME)) {
        nuecs_scene_interface_t *i = (nuecs_scene_interface_t*)interface;
        
        i->create = nuecs_scene_create_impl;
        i->destroy = nuecs_scene_destroy_impl;
        i->clear = nuecs_scene_clear_impl;
        i->progress = nuecs_scene_progress_impl;
        i->set_pipeline = nuecs_scene_set_pipeline_impl;
        i->serialize_json_object = nuecs_scene_serialize_json_object_impl;
        i->deserialize_json_object = nuecs_scene_deserialize_json_object_impl;
        i->save_json = nuecs_scene_save_json_impl;
        i->load_json = nuecs_scene_load_json_impl;
        i->debug_entities = nuecs_scene_debug_entities_impl;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_QUERY_INTERFACE_NAME)) {
        nuecs_query_interface_t *i = (nuecs_query_interface_t*)interface;
        
        i->create = nuecs_query_create_impl;
        i->destroy = nuecs_query_destroy_impl;
        i->resolve_chunks = nuecs_query_resolve_chunks_impl;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_ENTITY_INTERFACE_NAME)) {
        nuecs_entity_interface_t *i = (nuecs_entity_interface_t*)interface;
        
        i->create = nuecs_entity_create_impl;
        i->destroy = nuecs_entity_destroy_impl;
        i->add_component = nuecs_entity_add_component_impl;
        i->remove_component = nuecs_entity_remove_component_impl;
        i->serialize_json_object = nuecs_entity_serialize_json_object_impl;
        i->deserialize_json_object = nuecs_entity_deserialize_json_object_impl;
        i->remap = nuecs_entity_remap_impl;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_COMPONENT_INTERFACE_NAME)) {
        nuecs_component_interface_t *i = (nuecs_component_interface_t*)interface;
        
        i->build = nuecs_component_build_impl;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUECS_SYSTEM_INTERFACE_NAME)) {
        nuecs_system_interface_t *i = (nuecs_system_interface_t*)interface;
        
        i->build = nuecs_system_build_impl;
        i->compile_pipeline = nuecs_system_compile_pipeline_impl;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}