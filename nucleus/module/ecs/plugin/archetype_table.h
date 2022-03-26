#ifndef NUECS_ARCHETYPE_TABLE_H
#define NUECS_ARCHETYPE_TABLE_H

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/chunk.h>

nu_result_t nuecs_archetype_table_initialize(nu_array_t *table);
nu_result_t nuecs_archetype_table_terminate(nu_array_t table);
nu_result_t nuecs_archetype_table_get_next_chunk(
    nu_array_t table,
    nu_indexed_array_t queries,
    nuecs_archetype_data_t *archetype,
    nuecs_chunk_data_t **output
);

#endif