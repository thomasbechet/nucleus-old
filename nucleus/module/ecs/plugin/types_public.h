#ifndef NUECS_PUBLIC_DEFINITION_H
#define NUECS_PUBLIC_DEFINITION_H

#include <nucleus/nucleus.h>

#define NUECS_MAX_COMPONENT_PER_ENTITY 64

NU_DECLARE_HANDLE(nuecs_scene_t);
NU_DECLARE_HANDLE(nuecs_entity_t);
NU_DECLARE_HANDLE(nuecs_component_t);
NU_DECLARE_HANDLE(nuecs_system_t);
NU_DECLARE_HANDLE(nuecs_archetype_t);
NU_DECLARE_HANDLE(nuecs_query_t);

typedef void *nuecs_component_data_ptr_t;
typedef nu_result_t (*nuecs_component_serialize_json_pfn_t)(const nuecs_component_data_ptr_t, nu_json_object_t);
typedef nu_result_t (*nuecs_component_deserialize_json_pfn_t)(nu_json_object_t, nuecs_component_data_ptr_t);
typedef nu_result_t (*nuecs_component_initialize_pfn_t)(nuecs_component_data_ptr_t);
typedef nu_result_t (*nuecs_component_terminate_pfn_t)(nuecs_component_data_ptr_t);

typedef struct {
    const char *name;
    uint32_t size;
    nuecs_component_initialize_pfn_t initialize;
    nuecs_component_terminate_pfn_t terminate;
    nuecs_component_serialize_json_pfn_t serialize_json;
    nuecs_component_deserialize_json_pfn_t deserialize_json;
} nuecs_component_info_t;

typedef struct {
    nuecs_component_t *components;
    uint32_t component_count;
    nu_result_t (*update)(nuecs_component_data_ptr_t *components, uint32_t count);
} nuecs_system_info_t;

typedef struct {
    nuecs_component_t *components;
    nuecs_component_data_ptr_t *component_data;
    uint32_t component_count;
} nuecs_entity_info_t;

typedef enum {
    NUECS_QUERY_TYPE_CHUNK,
    NUECS_QUERY_TYPE_ENTITY
} nuecs_query_type_t;

typedef struct {
    nuecs_component_t *components;
    uint32_t component_count;
    nuecs_query_type_t type;
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