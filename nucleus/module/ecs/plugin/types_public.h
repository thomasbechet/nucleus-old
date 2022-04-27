#ifndef NUECS_PUBLIC_DEFINITION_H
#define NUECS_PUBLIC_DEFINITION_H

#include <nucleus/nucleus.h>

#define NUECS_MAX_COMPONENT_PER_ENTITY 32

NU_DECLARE_HANDLE(nuecs_scene_t);
NU_DECLARE_HANDLE(nuecs_entity_t);
NU_DECLARE_HANDLE(nuecs_entity_reference_t);
NU_DECLARE_HANDLE(nuecs_component_t);
NU_DECLARE_HANDLE(nuecs_system_t);
NU_DECLARE_HANDLE(nuecs_pipeline_t);
NU_DECLARE_HANDLE(nuecs_query_t);
NU_DECLARE_HANDLE(nuecs_command_buffer_t);
NU_DECLARE_HANDLE(nuecs_serialization_context_t);
NU_DECLARE_HANDLE(nuecs_deserialization_context_t);
NU_DECLARE_HANDLE(nuecs_transfer_context_t);

typedef void *nuecs_component_data_ptr_t;
typedef nu_result_t (*nuecs_component_serialize_json_pfn_t)(nuecs_component_data_ptr_t, nuecs_serialization_context_t, nu_json_object_t);
typedef nu_result_t (*nuecs_component_deserialize_json_pfn_t)(nuecs_component_data_ptr_t, nuecs_deserialization_context_t, nu_json_object_t);
typedef nu_result_t (*nuecs_component_transfer_pfn_t)(nuecs_component_data_ptr_t, nuecs_transfer_context_t);

typedef enum {
    NUECS_COMPONENT_FLAG_NONE         = 0x0,
    NUECS_COMPONENT_FLAG_SYSTEM_STATE = 0x1,
    NUECS_COMPONENT_FLAG_SERIALIZABLE = 0x2,
    NUECS_COMPONENT_FLAG_SINGLETON    = 0x4
} nuecs_component_flags_t;

typedef struct {
    const char *name;
    uint32_t size;
    uint8_t flags;
    nuecs_component_serialize_json_pfn_t serialize_json;
    nuecs_component_deserialize_json_pfn_t deserialize_json;
    nuecs_component_transfer_pfn_t transfer;
} nuecs_component_info_t;

typedef enum {
    NUECS_COMPONENT_ACCESS_READ,
    NUECS_COMPONENT_ACCESS_WRITE
} nuecs_component_access_t;

typedef struct {
    nuecs_component_t component;
    nuecs_component_access_t access;
} nuecs_component_dependency_t;

typedef nu_result_t (*nuecs_system_start_pfn_t)(void*, nuecs_scene_t);
typedef nu_result_t (*nuecs_system_stop_pfn_t)(void*, nuecs_scene_t);
typedef nu_result_t (*nuecs_system_update_pfn_t)(void*, nuecs_scene_t);

typedef struct {
    const char *name;
    uint32_t state_size;
    nuecs_component_dependency_t *dependencies;
    uint32_t dependency_count;
    nuecs_system_start_pfn_t start;
    nuecs_system_stop_pfn_t stop;
    nuecs_system_update_pfn_t update;
} nuecs_system_info_t;

typedef struct {
    const char *name;
    nuecs_system_t *systems;
    uint32_t system_count;
} nuecs_pipeline_info_t;

typedef struct {
    nuecs_component_t *components;
    nuecs_component_data_ptr_t *component_data;
    uint32_t component_count;
} nuecs_entity_info_t;

typedef struct {
    nuecs_component_t *include_components;
    uint32_t include_component_count;
    nuecs_component_t *exclude_components;
    uint32_t exclude_component_count;
} nuecs_query_info_t;

typedef struct {
    nuecs_component_data_ptr_t *components;
    uint32_t count;
} nuecs_query_chunk_view_t;

typedef struct {
    nuecs_query_chunk_view_t *chunks;
    uint32_t count;
} nuecs_query_result_t;

#endif