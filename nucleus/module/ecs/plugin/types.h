#ifndef NUECS_TYPES_H
#define NUECS_TYPES_H

#include <nucleus/module/ecs/module/interface.h>

#define NUECS_CHUNK_SIZE 32
#define NUECS_ARCHETYPE_MASK_LENGTH 4

#define NUECS_INSTANCEOF (nuecs_id_t)(1 << 63)
#define NUECS_CHILDOF    (nuecs_id_t)(1 << 62)

typedef struct {
    uint32_t size;
    uint32_t id;
    nu_string_t name;
} nuecs_component_type_t;

typedef struct {
    nu_array_t edges;
    nu_array_t chunks;
    nuecs_component_type_t **types;
    uint32_t *data_offsets;
    uint32_t *data_sizes;
    uint32_t type_count;

    uint32_t entity_count;
} nuecs_archetype_t;

typedef struct {
    nuecs_archetype_t *add;
    nuecs_archetype_t *remove;
    uint32_t type;
} nuecs_archetype_edge_t;

typedef struct {
    uint8_t *data;
    uint32_t *ids_to_indices;
    uint32_t *indices_to_ids;
    uint32_t size;
    uint32_t free_count;
} nuecs_chunk_data_t;

typedef struct {
    nuecs_archetype_t *archetype;
    uint32_t chunk;
    uint32_t row;
} nuecs_entity_data_t;

typedef struct {
    nu_array_t archetypes;
    nu_array_t types;
    nuecs_archetype_t *empty_archetype;
    uint32_t next_type_id;
    uint32_t id;
} nuecs_world_data_t;

#endif