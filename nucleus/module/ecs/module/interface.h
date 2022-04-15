/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_INTERFACE_H
#define NUECS_MODULE_INTERFACE_H

#include <nucleus/module/ecs/module/definition.h>

/* functions definition */
typedef nu_result_t (*__nuecs_archetype_debug_archetypes_pfn_t)(void);
typedef nu_result_t (*__nuecs_scene_create_pfn_t)(nuecs_scene_t* scene);
typedef nu_result_t (*__nuecs_scene_destroy_pfn_t)(nuecs_scene_t scene);
typedef nu_result_t (*__nuecs_scene_clear_pfn_t)(nuecs_scene_t scene);
typedef nu_result_t (*__nuecs_scene_set_pipeline_pfn_t)(nuecs_scene_t scene, nuecs_pipeline_t pipeline);
typedef nu_result_t (*__nuecs_scene_progress_pfn_t)(nuecs_scene_t scene);
typedef nu_result_t (*__nuecs_scene_serialize_json_object_pfn_t)(nuecs_scene_t scene, nu_json_object_t object);
typedef nu_result_t (*__nuecs_scene_deserialize_json_object_pfn_t)(nuecs_scene_t scene, nu_json_object_t object);
typedef nu_result_t (*__nuecs_scene_save_json_pfn_t)(nuecs_scene_t scene, const char* filename);
typedef nu_result_t (*__nuecs_scene_load_json_pfn_t)(nuecs_scene_t scene, const char* filename);
typedef nu_result_t (*__nuecs_scene_debug_entities_pfn_t)(nuecs_scene_t scene);
typedef nu_result_t (*__nuecs_query_create_pfn_t)(nuecs_scene_t scene, const nuecs_query_info_t* info, nuecs_query_t* handle);
typedef nu_result_t (*__nuecs_query_destroy_pfn_t)(nuecs_scene_t scene, nuecs_query_t query);
typedef nu_result_t (*__nuecs_query_resolve_pfn_t)(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_result_t* result);
typedef nu_result_t (*__nuecs_entity_create_pfn_t)(nuecs_scene_t scene, const nuecs_entity_info_t* info, nuecs_entity_t* handle);
typedef nu_result_t (*__nuecs_entity_destroy_pfn_t)(nuecs_scene_t scene, nuecs_entity_t entity);
typedef nu_result_t (*__nuecs_entity_add_component_pfn_t)(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component, nuecs_component_data_ptr_t component_data);
typedef nu_result_t (*__nuecs_entity_remove_component_pfn_t)(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component);
typedef nu_result_t (*__nuecs_entity_get_component_pfn_t)(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component, nuecs_component_data_ptr_t* data);
typedef nu_result_t (*__nuecs_entity_reference_bind_pfn_t)(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t entity);
typedef nu_result_t (*__nuecs_entity_reference_resolve_pfn_t)(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t* entity);
typedef nu_result_t (*__nuecs_entity_reference_serialize_json_object_pfn_t)(nuecs_entity_reference_t* handle, nuecs_serialization_context_t context, nu_json_object_t object, const char* name);
typedef nu_result_t (*__nuecs_entity_reference_deserialize_json_object_pfn_t)(nuecs_deserialization_context_t context, nu_json_object_t object, const char* name, nuecs_entity_reference_t* handle);
typedef nu_result_t (*__nuecs_component_build_pfn_t)(const nuecs_component_info_t* info, nuecs_component_t* handle);
typedef nu_result_t (*__nuecs_component_find_pfn_t)(const char* name, nuecs_component_t* handle);
typedef nu_result_t (*__nuecs_system_build_pfn_t)(const nuecs_system_info_t* info, nuecs_system_t* handle);
typedef nu_result_t (*__nuecs_pipeline_build_pfn_t)(const nuecs_pipeline_info_t* info, nuecs_pipeline_t* handle);

/* loader */
#ifdef NUECS_LOADER_IMPLEMENTATION
    __nuecs_archetype_debug_archetypes_pfn_t __nuecs_archetype_debug_archetypes = NULL;
    nu_result_t nuecs_archetype_interface_load(nu_module_t module)
    {
        nuecs_archetype_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_ARCHETYPE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_archetype_debug_archetypes = interface.debug_archetypes;
        }
        return result;
    }
    __nuecs_scene_create_pfn_t __nuecs_scene_create = NULL;
    __nuecs_scene_destroy_pfn_t __nuecs_scene_destroy = NULL;
    __nuecs_scene_clear_pfn_t __nuecs_scene_clear = NULL;
    __nuecs_scene_set_pipeline_pfn_t __nuecs_scene_set_pipeline = NULL;
    __nuecs_scene_progress_pfn_t __nuecs_scene_progress = NULL;
    __nuecs_scene_serialize_json_object_pfn_t __nuecs_scene_serialize_json_object = NULL;
    __nuecs_scene_deserialize_json_object_pfn_t __nuecs_scene_deserialize_json_object = NULL;
    __nuecs_scene_save_json_pfn_t __nuecs_scene_save_json = NULL;
    __nuecs_scene_load_json_pfn_t __nuecs_scene_load_json = NULL;
    __nuecs_scene_debug_entities_pfn_t __nuecs_scene_debug_entities = NULL;
    nu_result_t nuecs_scene_interface_load(nu_module_t module)
    {
        nuecs_scene_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_SCENE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_scene_create = interface.create;
            __nuecs_scene_destroy = interface.destroy;
            __nuecs_scene_clear = interface.clear;
            __nuecs_scene_set_pipeline = interface.set_pipeline;
            __nuecs_scene_progress = interface.progress;
            __nuecs_scene_serialize_json_object = interface.serialize_json_object;
            __nuecs_scene_deserialize_json_object = interface.deserialize_json_object;
            __nuecs_scene_save_json = interface.save_json;
            __nuecs_scene_load_json = interface.load_json;
            __nuecs_scene_debug_entities = interface.debug_entities;
        }
        return result;
    }
    __nuecs_query_create_pfn_t __nuecs_query_create = NULL;
    __nuecs_query_destroy_pfn_t __nuecs_query_destroy = NULL;
    __nuecs_query_resolve_pfn_t __nuecs_query_resolve = NULL;
    nu_result_t nuecs_query_interface_load(nu_module_t module)
    {
        nuecs_query_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_QUERY_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_query_create = interface.create;
            __nuecs_query_destroy = interface.destroy;
            __nuecs_query_resolve = interface.resolve;
        }
        return result;
    }
    __nuecs_entity_create_pfn_t __nuecs_entity_create = NULL;
    __nuecs_entity_destroy_pfn_t __nuecs_entity_destroy = NULL;
    __nuecs_entity_add_component_pfn_t __nuecs_entity_add_component = NULL;
    __nuecs_entity_remove_component_pfn_t __nuecs_entity_remove_component = NULL;
    __nuecs_entity_get_component_pfn_t __nuecs_entity_get_component = NULL;
    nu_result_t nuecs_entity_interface_load(nu_module_t module)
    {
        nuecs_entity_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_ENTITY_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_entity_create = interface.create;
            __nuecs_entity_destroy = interface.destroy;
            __nuecs_entity_add_component = interface.add_component;
            __nuecs_entity_remove_component = interface.remove_component;
            __nuecs_entity_get_component = interface.get_component;
        }
        return result;
    }
    __nuecs_entity_reference_bind_pfn_t __nuecs_entity_reference_bind = NULL;
    __nuecs_entity_reference_resolve_pfn_t __nuecs_entity_reference_resolve = NULL;
    __nuecs_entity_reference_serialize_json_object_pfn_t __nuecs_entity_reference_serialize_json_object = NULL;
    __nuecs_entity_reference_deserialize_json_object_pfn_t __nuecs_entity_reference_deserialize_json_object = NULL;
    nu_result_t nuecs_entity_reference_interface_load(nu_module_t module)
    {
        nuecs_entity_reference_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_ENTITY_REFERENCE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_entity_reference_bind = interface.bind;
            __nuecs_entity_reference_resolve = interface.resolve;
            __nuecs_entity_reference_serialize_json_object = interface.serialize_json_object;
            __nuecs_entity_reference_deserialize_json_object = interface.deserialize_json_object;
        }
        return result;
    }
    __nuecs_component_build_pfn_t __nuecs_component_build = NULL;
    __nuecs_component_find_pfn_t __nuecs_component_find = NULL;
    nu_result_t nuecs_component_interface_load(nu_module_t module)
    {
        nuecs_component_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_COMPONENT_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_component_build = interface.build;
            __nuecs_component_find = interface.find;
        }
        return result;
    }
    __nuecs_system_build_pfn_t __nuecs_system_build = NULL;
    nu_result_t nuecs_system_interface_load(nu_module_t module)
    {
        nuecs_system_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_SYSTEM_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_system_build = interface.build;
        }
        return result;
    }
    __nuecs_pipeline_build_pfn_t __nuecs_pipeline_build = NULL;
    nu_result_t nuecs_pipeline_interface_load(nu_module_t module)
    {
        nuecs_pipeline_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUECS_PIPELINE_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuecs_pipeline_build = interface.build;
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
    extern __nuecs_archetype_debug_archetypes_pfn_t __nuecs_archetype_debug_archetypes;
    nu_result_t nuecs_archetype_interface_load(nu_module_t module);
    extern __nuecs_scene_create_pfn_t __nuecs_scene_create;
    extern __nuecs_scene_destroy_pfn_t __nuecs_scene_destroy;
    extern __nuecs_scene_clear_pfn_t __nuecs_scene_clear;
    extern __nuecs_scene_set_pipeline_pfn_t __nuecs_scene_set_pipeline;
    extern __nuecs_scene_progress_pfn_t __nuecs_scene_progress;
    extern __nuecs_scene_serialize_json_object_pfn_t __nuecs_scene_serialize_json_object;
    extern __nuecs_scene_deserialize_json_object_pfn_t __nuecs_scene_deserialize_json_object;
    extern __nuecs_scene_save_json_pfn_t __nuecs_scene_save_json;
    extern __nuecs_scene_load_json_pfn_t __nuecs_scene_load_json;
    extern __nuecs_scene_debug_entities_pfn_t __nuecs_scene_debug_entities;
    nu_result_t nuecs_scene_interface_load(nu_module_t module);
    extern __nuecs_query_create_pfn_t __nuecs_query_create;
    extern __nuecs_query_destroy_pfn_t __nuecs_query_destroy;
    extern __nuecs_query_resolve_pfn_t __nuecs_query_resolve;
    nu_result_t nuecs_query_interface_load(nu_module_t module);
    extern __nuecs_entity_create_pfn_t __nuecs_entity_create;
    extern __nuecs_entity_destroy_pfn_t __nuecs_entity_destroy;
    extern __nuecs_entity_add_component_pfn_t __nuecs_entity_add_component;
    extern __nuecs_entity_remove_component_pfn_t __nuecs_entity_remove_component;
    extern __nuecs_entity_get_component_pfn_t __nuecs_entity_get_component;
    nu_result_t nuecs_entity_interface_load(nu_module_t module);
    extern __nuecs_entity_reference_bind_pfn_t __nuecs_entity_reference_bind;
    extern __nuecs_entity_reference_resolve_pfn_t __nuecs_entity_reference_resolve;
    extern __nuecs_entity_reference_serialize_json_object_pfn_t __nuecs_entity_reference_serialize_json_object;
    extern __nuecs_entity_reference_deserialize_json_object_pfn_t __nuecs_entity_reference_deserialize_json_object;
    nu_result_t nuecs_entity_reference_interface_load(nu_module_t module);
    extern __nuecs_component_build_pfn_t __nuecs_component_build;
    extern __nuecs_component_find_pfn_t __nuecs_component_find;
    nu_result_t nuecs_component_interface_load(nu_module_t module);
    extern __nuecs_system_build_pfn_t __nuecs_system_build;
    nu_result_t nuecs_system_interface_load(nu_module_t module);
    extern __nuecs_pipeline_build_pfn_t __nuecs_pipeline_build;
    nu_result_t nuecs_pipeline_interface_load(nu_module_t module);
    nu_result_t nuecs_interface_load_all(nu_module_t module);
#endif
#define nuecs_archetype_debug_archetypes NU_IDENTITY(__nuecs_archetype_debug_archetypes)
#define nuecs_scene_create NU_IDENTITY(__nuecs_scene_create)
#define nuecs_scene_destroy NU_IDENTITY(__nuecs_scene_destroy)
#define nuecs_scene_clear NU_IDENTITY(__nuecs_scene_clear)
#define nuecs_scene_set_pipeline NU_IDENTITY(__nuecs_scene_set_pipeline)
#define nuecs_scene_progress NU_IDENTITY(__nuecs_scene_progress)
#define nuecs_scene_serialize_json_object NU_IDENTITY(__nuecs_scene_serialize_json_object)
#define nuecs_scene_deserialize_json_object NU_IDENTITY(__nuecs_scene_deserialize_json_object)
#define nuecs_scene_save_json NU_IDENTITY(__nuecs_scene_save_json)
#define nuecs_scene_load_json NU_IDENTITY(__nuecs_scene_load_json)
#define nuecs_scene_debug_entities NU_IDENTITY(__nuecs_scene_debug_entities)
#define nuecs_query_create NU_IDENTITY(__nuecs_query_create)
#define nuecs_query_destroy NU_IDENTITY(__nuecs_query_destroy)
#define nuecs_query_resolve NU_IDENTITY(__nuecs_query_resolve)
#define nuecs_entity_create NU_IDENTITY(__nuecs_entity_create)
#define nuecs_entity_destroy NU_IDENTITY(__nuecs_entity_destroy)
#define nuecs_entity_add_component NU_IDENTITY(__nuecs_entity_add_component)
#define nuecs_entity_remove_component NU_IDENTITY(__nuecs_entity_remove_component)
#define nuecs_entity_get_component NU_IDENTITY(__nuecs_entity_get_component)
#define nuecs_entity_reference_bind NU_IDENTITY(__nuecs_entity_reference_bind)
#define nuecs_entity_reference_resolve NU_IDENTITY(__nuecs_entity_reference_resolve)
#define nuecs_entity_reference_serialize_json_object NU_IDENTITY(__nuecs_entity_reference_serialize_json_object)
#define nuecs_entity_reference_deserialize_json_object NU_IDENTITY(__nuecs_entity_reference_deserialize_json_object)
#define nuecs_component_build NU_IDENTITY(__nuecs_component_build)
#define nuecs_component_find NU_IDENTITY(__nuecs_component_find)
#define nuecs_system_build NU_IDENTITY(__nuecs_system_build)
#define nuecs_pipeline_build NU_IDENTITY(__nuecs_pipeline_build)

#endif