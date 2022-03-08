#ifndef NUECS_ARCHETYPE_TABLE_H
#define NUECS_ARCHETYPE_TABLE_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_archetype_table_initialize(nu_array_t *table);
nu_result_t nuecs_archetype_table_terminate(nu_array_t table);
nu_result_t nuecs_archetype_table_get_entry(
    nu_array_t table, 
    const nuecs_archetype_data_t *archetype,
    nuecs_archetype_entry_data_t **entry
);
nu_result_t nuecs_archetype_table_get_next_chunk(
    nu_array_t table, 
    nuecs_archetype_data_t *archetype, 
    nuecs_chunk_data_t **chunk
);

#endif