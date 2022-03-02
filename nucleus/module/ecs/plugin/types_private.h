#ifndef NUECS_TYPES_H
#define NUECS_TYPES_H

#include <nucleus/module/ecs/plugin/types_public.h>

#define NUECS_LOGGER_NAME "ECS"

typedef struct {
    uint32_t size;
    nu_string_t name;
    uint32_t id;
    nuecs_component_serialize_json_pfn_t serialize_json;
    nuecs_component_deserialize_json_pfn_t deserialize_json;
} nuecs_component_data_t;

typedef struct {
    nu_array_t edges;
    uint32_t component_count;
    uint32_t *data_sizes;
    uint32_t *component_ids;
    uint32_t index;
} nuecs_archetype_data_t;

typedef struct {
    nuecs_archetype_data_t *add;
    nuecs_archetype_data_t *remove;
    uint32_t component_id;
} nuecs_archetype_edge_t;

typedef struct {
    nuecs_archetype_data_t *archetype;
    uint32_t size;
    uint32_t frame_size;
    uint32_t free_count;
    uint32_t *indice_table;
    nuecs_component_data_ptr_t *component_list_ptrs;
} nuecs_chunk_data_t;

typedef struct {
    nuecs_archetype_data_t *archetype;
    nu_array_t chunks;
} nuecs_archetype_chunks_data_t;

typedef struct {
    nuecs_chunk_data_t *chunk;
    nuecs_component_data_ptr_t components[1];
} nuecs_chunk_view_data_t;

typedef struct {
    nuecs_chunk_data_t *chunk;
    uint32_t id;
} nuecs_entity_entry_t;

typedef struct {
    nu_array_t entries;         /* nuecs_entity_entry_t */
    nu_array_t free_entries;    /* uint32_t */
    nu_array_t deleted_entries; /* uint32_t */
    nu_array_t archetype_table; /* nuecs_archetype_chunks_t */
    uint32_t id;
} nuecs_scene_data_t;

typedef struct {
    nu_indexed_array_t scenes;
} nuecs_scene_manager_data_t;

typedef struct {
    nu_array_t archetypes;
    nuecs_archetype_data_t *empty_archetype;
    nu_indexed_array_t components;
    uint32_t next_archetype_index;
} nuecs_component_manager_data_t;

typedef struct {
    nu_array_t chunk_views;
    uint32_t *component_ids;
    uint32_t component_count;
    nu_result_t (*update)(nuecs_component_data_ptr_t *components, uint32_t count);
    uint32_t id;
} nuecs_system_data_t;

#endif