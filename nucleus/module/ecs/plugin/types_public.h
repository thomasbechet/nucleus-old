#ifndef NUECS_PUBLIC_DEFINITION_H
#define NUECS_PUBLIC_DEFINITION_H

#include <nucleus/nucleus.h>

#define NUECS_MAX_COMPONENT_PER_ENTITY 32

NU_DECLARE_HANDLE(nuecs_scene_t);
NU_DECLARE_HANDLE(nuecs_entity_t);
NU_DECLARE_HANDLE(nuecs_component_t);
NU_DECLARE_HANDLE(nuecs_system_t);
NU_DECLARE_HANDLE(nuecs_pipeline_t);
NU_DECLARE_HANDLE(nuecs_query_t);
NU_DECLARE_HANDLE(nuecs_serialization_context_t);
NU_DECLARE_HANDLE(nuecs_deserialization_context_t);
NU_DECLARE_HANDLE(nuecs_transfer_context_t);

typedef void *nuecs_component_data_ptr_t;
typedef nu_result_t (*nuecs_component_serialize_json_pfn_t)(nuecs_component_data_ptr_t, nuecs_serialization_context_t, nu_json_object_t);
typedef nu_result_t (*nuecs_component_deserialize_json_pfn_t)(nuecs_component_data_ptr_t, nuecs_deserialization_context_t, nu_json_object_t);
typedef nu_result_t (*nuecs_component_transfer_pfn_t)(nuecs_component_data_ptr_t, nuecs_transfer_context_t);
typedef nu_result_t (*nuecs_system_callback_pfn_t)(nuecs_scene_t, nuecs_query_t);

typedef struct {
    const char *name;
    uint32_t size;
    nuecs_component_serialize_json_pfn_t serialize_json;
    nuecs_component_deserialize_json_pfn_t deserialize_json;
    nuecs_component_transfer_pfn_t transfer;
} nuecs_component_info_t;

typedef struct {
    const char *name;
    nuecs_component_t *components;
    uint32_t component_count;
    nuecs_system_callback_pfn_t callback;
} nuecs_system_info_t;

typedef struct nuecs_pipeline_node_t {
    nuecs_system_t system;
    struct nuecs_pipeline_node_t *dependencies;
    uint32_t dependency_count;
} nuecs_pipeline_node_t;

typedef struct {
    nuecs_pipeline_node_t *targets;
    uint32_t target_count;
} nuecs_pipeline_info_t;

typedef struct {
    nuecs_component_t *components;
    nuecs_component_data_ptr_t *component_data;
    uint32_t component_count;
} nuecs_entity_info_t;

typedef struct {
    nuecs_component_t *components;
    uint32_t component_count;
} nuecs_query_info_t;

typedef struct {
    nuecs_component_data_ptr_t *components;
    uint32_t count;
} nuecs_query_chunk_view_t;

typedef struct {
    nuecs_query_chunk_view_t *views;
    uint32_t view_count;
} nuecs_query_chunks_t;

#endif