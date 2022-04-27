/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_DEFINITION_H
#define NUECS_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/ecs/plugin/types_public.h>

/* module */
#define NUECS_MODULE_NAME "nucleus-ecs"
#define NUECS_MODULE_ID 14

/* interface */
#define NUECS_ARCHETYPE_INTERFACE_NAME "nuecs_archetype_interface"
typedef nu_result_t (*nuecs_archetype_debug_archetypes_pfn_t)(void);

typedef struct {
    nuecs_archetype_debug_archetypes_pfn_t debug_archetypes;
} nuecs_archetype_interface_t;

#define NUECS_SCENE_INTERFACE_NAME "nuecs_scene_interface"
typedef nu_result_t (*nuecs_scene_create_pfn_t)(nuecs_scene_t* scene);
typedef nu_result_t (*nuecs_scene_destroy_pfn_t)(nuecs_scene_t scene);
typedef nu_result_t (*nuecs_scene_clear_pfn_t)(nuecs_scene_t scene);
typedef nu_result_t (*nuecs_scene_set_pipeline_pfn_t)(nuecs_scene_t scene, nuecs_pipeline_t pipeline);
typedef nu_result_t (*nuecs_scene_progress_pfn_t)(nuecs_scene_t scene);
typedef nu_result_t (*nuecs_scene_serialize_json_object_pfn_t)(nuecs_scene_t scene, nu_json_object_t object);
typedef nu_result_t (*nuecs_scene_deserialize_json_object_pfn_t)(nuecs_scene_t scene, nu_json_object_t object);
typedef nu_result_t (*nuecs_scene_save_json_pfn_t)(nuecs_scene_t scene, const char* filename);
typedef nu_result_t (*nuecs_scene_load_json_pfn_t)(nuecs_scene_t scene, const char* filename);
typedef nu_result_t (*nuecs_scene_debug_entities_pfn_t)(nuecs_scene_t scene);

typedef struct {
    nuecs_scene_create_pfn_t create;
    nuecs_scene_destroy_pfn_t destroy;
    nuecs_scene_clear_pfn_t clear;
    nuecs_scene_set_pipeline_pfn_t set_pipeline;
    nuecs_scene_progress_pfn_t progress;
    nuecs_scene_serialize_json_object_pfn_t serialize_json_object;
    nuecs_scene_deserialize_json_object_pfn_t deserialize_json_object;
    nuecs_scene_save_json_pfn_t save_json;
    nuecs_scene_load_json_pfn_t load_json;
    nuecs_scene_debug_entities_pfn_t debug_entities;
} nuecs_scene_interface_t;

#define NUECS_QUERY_INTERFACE_NAME "nuecs_query_interface"
typedef nu_result_t (*nuecs_query_create_pfn_t)(nuecs_scene_t scene, const nuecs_query_info_t* info, nuecs_query_t* handle);
typedef nu_result_t (*nuecs_query_destroy_pfn_t)(nuecs_scene_t scene, nuecs_query_t query);
typedef nu_result_t (*nuecs_query_resolve_pfn_t)(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_result_t* result);

typedef struct {
    nuecs_query_create_pfn_t create;
    nuecs_query_destroy_pfn_t destroy;
    nuecs_query_resolve_pfn_t resolve;
} nuecs_query_interface_t;

#define NUECS_ENTITY_INTERFACE_NAME "nuecs_entity_interface"
typedef nu_result_t (*nuecs_entity_create_pfn_t)(nuecs_scene_t scene, const nuecs_entity_info_t* info, nuecs_entity_t* handle);
typedef nu_result_t (*nuecs_entity_destroy_pfn_t)(nuecs_scene_t scene, nuecs_entity_t entity);
typedef nu_result_t (*nuecs_entity_add_component_pfn_t)(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component, nuecs_component_data_ptr_t component_data);
typedef nu_result_t (*nuecs_entity_remove_component_pfn_t)(nuecs_scene_t scene, nuecs_entity_t* entity, nuecs_component_t component);
typedef nu_result_t (*nuecs_entity_get_component_pfn_t)(nuecs_scene_t scene, nuecs_entity_t entity, nuecs_component_t component, nuecs_component_data_ptr_t* data);

typedef struct {
    nuecs_entity_create_pfn_t create;
    nuecs_entity_destroy_pfn_t destroy;
    nuecs_entity_add_component_pfn_t add_component;
    nuecs_entity_remove_component_pfn_t remove_component;
    nuecs_entity_get_component_pfn_t get_component;
} nuecs_entity_interface_t;

#define NUECS_ENTITY_REFERENCE_INTERFACE_NAME "nuecs_entity_reference_interface"
typedef nu_result_t (*nuecs_entity_reference_bind_pfn_t)(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t entity);
typedef nu_result_t (*nuecs_entity_reference_resolve_pfn_t)(nuecs_scene_t scene, nuecs_entity_reference_t* handle, nuecs_entity_t* entity);
typedef nu_result_t (*nuecs_entity_reference_serialize_json_object_pfn_t)(nuecs_entity_reference_t* handle, nuecs_serialization_context_t context, nu_json_object_t object, const char* name);
typedef nu_result_t (*nuecs_entity_reference_deserialize_json_object_pfn_t)(nuecs_deserialization_context_t context, nu_json_object_t object, const char* name, nuecs_entity_reference_t* handle);

typedef struct {
    nuecs_entity_reference_bind_pfn_t bind;
    nuecs_entity_reference_resolve_pfn_t resolve;
    nuecs_entity_reference_serialize_json_object_pfn_t serialize_json_object;
    nuecs_entity_reference_deserialize_json_object_pfn_t deserialize_json_object;
} nuecs_entity_reference_interface_t;

#define NUECS_COMPONENT_INTERFACE_NAME "nuecs_component_interface"
typedef nu_result_t (*nuecs_component_build_pfn_t)(const nuecs_component_info_t* info, nuecs_component_t* handle);
typedef nu_result_t (*nuecs_component_find_pfn_t)(const char* name, nuecs_component_t* handle);

typedef struct {
    nuecs_component_build_pfn_t build;
    nuecs_component_find_pfn_t find;
} nuecs_component_interface_t;

#define NUECS_SYSTEM_INTERFACE_NAME "nuecs_system_interface"
typedef nu_result_t (*nuecs_system_build_pfn_t)(const nuecs_system_info_t* info, nuecs_system_t* handle);

typedef struct {
    nuecs_system_build_pfn_t build;
} nuecs_system_interface_t;

#define NUECS_PIPELINE_INTERFACE_NAME "nuecs_pipeline_interface"
typedef nu_result_t (*nuecs_pipeline_build_pfn_t)(const nuecs_pipeline_info_t* info, nuecs_pipeline_t* handle);

typedef struct {
    nuecs_pipeline_build_pfn_t build;
} nuecs_pipeline_interface_t;


/* plugin */
#define NUECS_SCENE_PLUGIN_NAME "nuecs_scene_plugin"

#endif