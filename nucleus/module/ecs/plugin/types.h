#ifndef NUECS_TYPES_H
#define NUECS_TYPES_H

#include <nucleus/module/ecs/module/interface.h>

#define NUECS_INSTANCEOF (nuecs_id_t)(1 << 63)
#define NUECS_CHILDOF    (nuecs_id_t)(1 << 62)

typedef struct {
    uint32_t size;
    uint32_t component_id;
    nu_string_t name;
    uint32_t id;
} nuecs_component_data_t;

typedef struct {
    nu_array_t edges;
    nu_array_t chunks;
    nu_array_t systems;
    uint32_t component_count;
    uint32_t *data_sizes;
    uint32_t *component_ids;
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
    nuecs_chunk_data_t *chunk;
    nuecs_component_data_ptr_t components[1];
} nuecs_chunk_view_t;

typedef struct {
    nuecs_chunk_data_t *chunk;
    uint32_t chunk_id;
} nuecs_entity_entry_t;

typedef struct {
    nu_array_t archetypes;
    nu_array_t entries;
    nu_array_t free_entries;
    nu_array_t deleted_entries;
    nu_indexed_array_t components;
    nu_indexed_array_t systems;
    nuecs_archetype_data_t *empty_archetype;
    uint32_t next_component_id;
    uint32_t id;
} nuecs_world_data_t;

typedef struct {
    nu_array_t chunks;
    uint32_t *component_ids;
    uint32_t component_count;
    nu_result_t (*update)(nuecs_component_data_ptr_t *components, uint32_t count);
    uint32_t id;
} nuecs_system_data_t;

#endif