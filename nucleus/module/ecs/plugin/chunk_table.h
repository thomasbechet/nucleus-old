#ifndef NUECS_CHUNK_TABLE_H
#define NUECS_CHUNK_TABLE_H

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/chunk.h>

typedef struct {
    nuecs_archetype_data_t *archetype;
    nu_array_t chunks;
    nu_array_t notify_queries;
} nuecs_archetype_slot_t;

typedef struct {
    nu_array_t slots;
} nuecs_chunk_table_t;

nu_result_t nuecs_chunk_table_initialize(nuecs_chunk_table_t *table);
nu_result_t nuecs_chunk_table_terminate(nuecs_chunk_table_t *table);
nu_result_t nuecs_chunk_table_get_next_chunk(
    nuecs_chunk_table_t *table,
    nu_indexed_array_t queries,
    nuecs_archetype_data_t *archetype,
    nuecs_chunk_data_t **output,
    bool *new_chunk
);

#endif