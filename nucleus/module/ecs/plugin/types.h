#ifndef NUECS_TYPES_H
#define NUECS_TYPES_H

#include <nucleus/module/ecs/module/interface.h>

#define NUECS_CHUNK_SIZE 32
#define NUECS_ARCHETYPE_MASK_LENGTH 4

// typedef uint64_t nuecs_archetype_mask_t[4];

typedef uint64_t nuecs_id_t;

#define NUECS_INSTANCEOF (nuecs_id_t)(1 << 63)
#define NUECS_CHILDOF    (nuecs_id_t)(1 << 62)

typedef struct {
    void *components;
    uint32_t count;
} nuecs_chunk_data_t;

struct nuecs_archetype_data_t;

typedef struct {
    nuecs_archetype_data_t *add;
    nuecs_archetype_data_t *remove;
    nuecs_id_t type;
    uint32_t offset;
} nuecs_archetype_type_data_t;

typedef struct {
    nu_array_t types;
    nu_array_t chunks;
} nuecs_archetype_data_t;

typedef struct {
    nu_array_t archetype_refs;
} nuecs_system_data_t;

typedef struct {
    nu_array_t archetypes;
    uint32_t id;
} nuecs_world_data_t;

#endif