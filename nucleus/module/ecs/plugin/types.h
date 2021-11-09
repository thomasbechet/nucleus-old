#ifndef NUECS_TYPES_H
#define NUECS_TYPES_H

#include <nucleus/module/ecs/module/interface.h>

#define NUECS_CHUNK_SIZE 32
#define NUECS_ARCHETYPE_MASK_LENGTH 4

typedef uint64_t nuecs_id_t;

#define NUECS_INSTANCEOF (nuecs_id_t)(1 << 63)
#define NUECS_CHILDOF    (nuecs_id_t)(1 << 62)

typedef struct {
    nu_array_t edges;
    nu_array_t chunks;
    nuecs_id_t *types;
    uint32_t type_count;
} nuecs_archetype_t;

typedef struct {
    nuecs_archetype_t *add;
    nuecs_archetype_t *remove;
    nuecs_id_t type;
} nuecs_archetype_edge_t;

typedef struct {
    void *components;
    uint32_t count;
} nuecs_chunk_data_t;

typedef struct {
    uint32_t size;
    nu_string_t name;
} nuecs_component_data_t;

typedef struct {
    nu_array_t archetypes;
    nu_indexed_array_t types;
    uint32_t id;
} nuecs_world_data_t;

#endif