#ifndef NUECS_CHUNK_TABLE_H
#define NUECS_CHUNK_TABLE_H

#include <nucleus/nucleus.h>

typedef struct nuecs_query_table nuecs_query_table_t;
typedef struct nuecs_chunk_data nuecs_chunk_data_t;
typedef struct nuecs_archetype_data nuecs_archetype_data_t;

typedef struct nuecs_chunk_archetype_slot {
    nu_array_t chunks; /* nuecs_chunk_data_t* */
} nuecs_chunk_archetype_slot_t;

typedef struct nuecs_chunk_table {
    nu_array_t archetype_slots;  /* nuecs_archetype_slot_t */
    nu_array_t chunk_references; /* nuecs_chunk_data_t* */
} nuecs_chunk_table_t;

nu_result_t nuecs_chunk_table_initialize(nuecs_chunk_table_t *table);
nu_result_t nuecs_chunk_table_terminate(nuecs_chunk_table_t *table);

nu_result_t nuecs_chunk_table_get_next_chunk(
    nuecs_chunk_table_t *table,
    nuecs_query_table_t *query_table,
    nuecs_archetype_data_t *archetype,
    nuecs_chunk_data_t **chunk
);
nu_result_t nuecs_chunk_table_get_chunk(
    nuecs_chunk_table_t *table,
    uint32_t chunk_id,
    nuecs_chunk_data_t **chunk
);

#endif