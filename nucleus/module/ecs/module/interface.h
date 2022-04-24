/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_INTERFACE_H
#define NUECS_MODULE_INTERFACE_H

#include <nucleus/module/ecs/module/definition.h>

/* loader */
#ifdef NUECS_LOADER_IMPLEMENTATION
    nuecs_archetype_debug_archetypes_pfn_t nuecs_archetype_debug_archetypes = NULL;
    nu_result_t nuecs_archetype_interface_load(nu_module_t module)
    {
        nuecs_archetype_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_ARCHETYPE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_archetype_debug_archetypes = interface.debug_archetypes;
        }
        return result;
    }
    nuecs_scene_create_pfn_t nuecs_scene_create = NULL;
    nuecs_scene_destroy_pfn_t nuecs_scene_destroy = NULL;
    nuecs_scene_clear_pfn_t nuecs_scene_clear = NULL;
    nuecs_scene_set_pipeline_pfn_t nuecs_scene_set_pipeline = NULL;
    nuecs_scene_progress_pfn_t nuecs_scene_progress = NULL;
    nuecs_scene_serialize_json_object_pfn_t nuecs_scene_serialize_json_object = NULL;
    nuecs_scene_deserialize_json_object_pfn_t nuecs_scene_deserialize_json_object = NULL;
    nuecs_scene_save_json_pfn_t nuecs_scene_save_json = NULL;
    nuecs_scene_load_json_pfn_t nuecs_scene_load_json = NULL;
    nuecs_scene_debug_entities_pfn_t nuecs_scene_debug_entities = NULL;
    nu_result_t nuecs_scene_interface_load(nu_module_t module)
    {
        nuecs_scene_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_SCENE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_scene_create = interface.create;
            nuecs_scene_destroy = interface.destroy;
            nuecs_scene_clear = interface.clear;
            nuecs_scene_set_pipeline = interface.set_pipeline;
            nuecs_scene_progress = interface.progress;
            nuecs_scene_serialize_json_object = interface.serialize_json_object;
            nuecs_scene_deserialize_json_object = interface.deserialize_json_object;
            nuecs_scene_save_json = interface.save_json;
            nuecs_scene_load_json = interface.load_json;
            nuecs_scene_debug_entities = interface.debug_entities;
        }
        return result;
    }
    nuecs_query_create_pfn_t nuecs_query_create = NULL;
    nuecs_query_destroy_pfn_t nuecs_query_destroy = NULL;
    nuecs_query_resolve_pfn_t nuecs_query_resolve = NULL;
    nu_result_t nuecs_query_interface_load(nu_module_t module)
    {
        nuecs_query_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_QUERY_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_query_create = interface.create;
            nuecs_query_destroy = interface.destroy;
            nuecs_query_resolve = interface.resolve;
        }
        return result;
    }
    nuecs_entity_create_pfn_t nuecs_entity_create = NULL;
    nuecs_entity_destroy_pfn_t nuecs_entity_destroy = NULL;
    nuecs_entity_add_component_pfn_t nuecs_entity_add_component = NULL;
    nuecs_entity_remove_component_pfn_t nuecs_entity_remove_component = NULL;
    nuecs_entity_get_component_pfn_t nuecs_entity_get_component = NULL;
    nu_result_t nuecs_entity_interface_load(nu_module_t module)
    {
        nuecs_entity_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_ENTITY_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_entity_create = interface.create;
            nuecs_entity_destroy = interface.destroy;
            nuecs_entity_add_component = interface.add_component;
            nuecs_entity_remove_component = interface.remove_component;
            nuecs_entity_get_component = interface.get_component;
        }
        return result;
    }
    nuecs_entity_reference_bind_pfn_t nuecs_entity_reference_bind = NULL;
    nuecs_entity_reference_resolve_pfn_t nuecs_entity_reference_resolve = NULL;
    nuecs_entity_reference_serialize_json_object_pfn_t nuecs_entity_reference_serialize_json_object = NULL;
    nuecs_entity_reference_deserialize_json_object_pfn_t nuecs_entity_reference_deserialize_json_object = NULL;
    nu_result_t nuecs_entity_reference_interface_load(nu_module_t module)
    {
        nuecs_entity_reference_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_ENTITY_REFERENCE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_entity_reference_bind = interface.bind;
            nuecs_entity_reference_resolve = interface.resolve;
            nuecs_entity_reference_serialize_json_object = interface.serialize_json_object;
            nuecs_entity_reference_deserialize_json_object = interface.deserialize_json_object;
        }
        return result;
    }
    nuecs_component_build_pfn_t nuecs_component_build = NULL;
    nuecs_component_find_pfn_t nuecs_component_find = NULL;
    nu_result_t nuecs_component_interface_load(nu_module_t module)
    {
        nuecs_component_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_COMPONENT_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_component_build = interface.build;
            nuecs_component_find = interface.find;
        }
        return result;
    }
    nuecs_system_build_pfn_t nuecs_system_build = NULL;
    nu_result_t nuecs_system_interface_load(nu_module_t module)
    {
        nuecs_system_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_SYSTEM_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_system_build = interface.build;
        }
        return result;
    }
    nuecs_pipeline_build_pfn_t nuecs_pipeline_build = NULL;
    nu_result_t nuecs_pipeline_interface_load(nu_module_t module)
    {
        nuecs_pipeline_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_PIPELINE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuecs_pipeline_build = interface.build;
        }
        return result;
    }
    nu_result_t nuecs_interface_load_all(nu_module_t module)
    {
        nu_result_t result = NU_SUCCESS;
        result &= nuecs_archetype_interface_load(module);
        result &= nuecs_scene_interface_load(module);
        result &= nuecs_query_interface_load(module);
        result &= nuecs_entity_interface_load(module);
        result &= nuecs_entity_reference_interface_load(module);
        result &= nuecs_component_interface_load(module);
        result &= nuecs_system_interface_load(module);
        result &= nuecs_pipeline_interface_load(module);
        return result;
    }
#else
    extern nuecs_archetype_debug_archetypes_pfn_t nuecs_archetype_debug_archetypes;
    nu_result_t nuecs_archetype_interface_load(nu_module_t module);
    extern nuecs_scene_create_pfn_t nuecs_scene_create;
    extern nuecs_scene_destroy_pfn_t nuecs_scene_destroy;
    extern nuecs_scene_clear_pfn_t nuecs_scene_clear;
    extern nuecs_scene_set_pipeline_pfn_t nuecs_scene_set_pipeline;
    extern nuecs_scene_progress_pfn_t nuecs_scene_progress;
    extern nuecs_scene_serialize_json_object_pfn_t nuecs_scene_serialize_json_object;
    extern nuecs_scene_deserialize_json_object_pfn_t nuecs_scene_deserialize_json_object;
    extern nuecs_scene_save_json_pfn_t nuecs_scene_save_json;
    extern nuecs_scene_load_json_pfn_t nuecs_scene_load_json;
    extern nuecs_scene_debug_entities_pfn_t nuecs_scene_debug_entities;
    nu_result_t nuecs_scene_interface_load(nu_module_t module);
    extern nuecs_query_create_pfn_t nuecs_query_create;
    extern nuecs_query_destroy_pfn_t nuecs_query_destroy;
    extern nuecs_query_resolve_pfn_t nuecs_query_resolve;
    nu_result_t nuecs_query_interface_load(nu_module_t module);
    extern nuecs_entity_create_pfn_t nuecs_entity_create;
    extern nuecs_entity_destroy_pfn_t nuecs_entity_destroy;
    extern nuecs_entity_add_component_pfn_t nuecs_entity_add_component;
    extern nuecs_entity_remove_component_pfn_t nuecs_entity_remove_component;
    extern nuecs_entity_get_component_pfn_t nuecs_entity_get_component;
    nu_result_t nuecs_entity_interface_load(nu_module_t module);
    extern nuecs_entity_reference_bind_pfn_t nuecs_entity_reference_bind;
    extern nuecs_entity_reference_resolve_pfn_t nuecs_entity_reference_resolve;
    extern nuecs_entity_reference_serialize_json_object_pfn_t nuecs_entity_reference_serialize_json_object;
    extern nuecs_entity_reference_deserialize_json_object_pfn_t nuecs_entity_reference_deserialize_json_object;
    nu_result_t nuecs_entity_reference_interface_load(nu_module_t module);
    extern nuecs_component_build_pfn_t nuecs_component_build;
    extern nuecs_component_find_pfn_t nuecs_component_find;
    nu_result_t nuecs_component_interface_load(nu_module_t module);
    extern nuecs_system_build_pfn_t nuecs_system_build;
    nu_result_t nuecs_system_interface_load(nu_module_t module);
    extern nuecs_pipeline_build_pfn_t nuecs_pipeline_build;
    nu_result_t nuecs_pipeline_interface_load(nu_module_t module);
    nu_result_t nuecs_interface_load_all(nu_module_t module);
#endif

#endif