#ifndef NUECS_TYPES_H
#define NUECS_TYPES_H

#include <nucleus/module/ecs/module/interface.h>

#define NUECS_CHUNK_SIZE 32
#define NUECS_ARCHETYPE_MASK_LENGTH 4

typedef uint64_t nuecs_archetype_mask_t[4];

typedef struct {
    void *components;
    uint32_t count;
} nuecs_chunk_data_t;

typedef struct {
    nuecs_archetype_mask_t flags;
    nu_array_t chunks;
    uint32_t *sizes;
    uint32_t component_count;
} nuecs_archetype_data_t;

typedef struct {
    nu_array_t archetype_refs;
} nuecs_system_data_t;

typedef struct {
    nu_array_t archetypes;
    uint32_t id;
} nuecs_world_data_t;

#endif