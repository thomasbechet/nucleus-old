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

typedef struct {
    nu_result_t (*debug_archetypes)(void);
} nuecs_archetype_interface_t;

#define NUECS_SCENE_INTERFACE_NAME "nuecs_scene_interface"

typedef struct {
    nu_result_t (*create)(nuecs_scene_t*);
    nu_result_t (*destroy)(nuecs_scene_t);
    nu_result_t (*clear)(nuecs_scene_t);
    nu_result_t (*progress)(nuecs_scene_t);
    nu_result_t (*set_pipeline)(nuecs_scene_t, nuecs_pipeline_t);
    nu_result_t (*serialize_json_object)(nuecs_scene_t, nu_json_object_t);
    nu_result_t (*deserialize_json_object)(nuecs_scene_t, nu_json_object_t);
    nu_result_t (*save_json)(nuecs_scene_t, const char*);
    nu_result_t (*load_json)(nuecs_scene_t, const char*);
    nu_result_t (*debug_entities)(nuecs_scene_t);
} nuecs_scene_interface_t;

#define NUECS_QUERY_INTERFACE_NAME "nuecs_query_interface"

typedef struct {
    nu_result_t (*create)(nuecs_scene_t, const nuecs_query_info_t*, nuecs_query_t*);
    nu_result_t (*destroy)(nuecs_scene_t, nuecs_query_t);
    nu_result_t (*resolve_chunks)(nuecs_scene_t, nuecs_query_t, nuecs_query_chunks_t*);
} nuecs_query_interface_t;

#define NUECS_ENTITY_INTERFACE_NAME "nuecs_entity_interface"

typedef struct {
    nu_result_t (*create)(nuecs_scene_t, const nuecs_entity_info_t*, nuecs_entity_t*);
    nu_result_t (*destroy)(nuecs_scene_t, nuecs_entity_t);
    nu_result_t (*add_component)(nuecs_scene_t, nuecs_entity_t, nuecs_component_t, nuecs_component_data_ptr_t);
    nu_result_t (*remove_component)(nuecs_scene_t, nuecs_entity_t, nuecs_component_t);
    nu_result_t (*serialize_json_object)(nuecs_entity_t, nuecs_serialization_context_t, nu_json_object_t, const char*);
    nu_result_t (*deserialize_json_object)(nuecs_deserialization_context_t, nu_json_object_t, const char*, nuecs_entity_t*);
    nu_result_t (*remap)(nuecs_transfer_context_t, nuecs_entity_t*);
} nuecs_entity_interface_t;

#define NUECS_COMPONENT_INTERFACE_NAME "nuecs_component_interface"

typedef struct {
    nu_result_t (*build)(const nuecs_component_info_t*, nuecs_component_t*);
} nuecs_component_interface_t;

#define NUECS_SYSTEM_INTERFACE_NAME "nuecs_system_interface"

typedef struct {
    nu_result_t (*build)(nuecs_system_info_t*, nuecs_system_t*);
    nu_result_t (*compile_pipeline)(nuecs_pipeline_info_t*, nuecs_pipeline_t*);
} nuecs_system_interface_t;


/* plugin */
#define NUECS_SCENE_PLUGIN_NAME "nuecs_scene_plugin"

#endif