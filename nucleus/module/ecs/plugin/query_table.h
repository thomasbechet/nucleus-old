#ifndef NUECS_QUERY_TABLE_H
#define NUECS_QUERY_TABLE_H

#include <nucleus/nucleus.h>

typedef struct nuecs_archetype_data nuecs_archetype_data_t;
typedef struct nuecs_chunk_table nuecs_chunk_table_t;
typedef struct nuecs_chunk_data nuecs_chunk_data_t;
typedef struct nuecs_query_data nuecs_query_data_t;

typedef struct nuecs_query_archetype_slot {
    nuecs_archetype_data_t *archetype;
    nu_array_t query_references; /* nuecs_query_data_t* */
} nuecs_query_archetype_slot_t;

typedef struct nuecs_query_table {
    nu_array_t archetype_slots;  /* nuecs_archetype_slot_t */
    nu_array_t queries;          /* nuecs_query_data_t* */
} nuecs_query_table_t;

nu_result_t nuecs_query_table_initialize(nuecs_query_table_t *table);
nu_result_t nuecs_query_table_terminate(nuecs_query_table_t *table);

nu_result_t nuecs_query_table_create_query(
    nuecs_query_table_t *table,
    nuecs_chunk_table_t *chunk_table,
    uint32_t *include_components, 
    uint32_t include_component_count,
    uint32_t *exclude_components,
    uint32_t exclude_component_count,
    nuecs_query_data_t **query
);
nu_result_t nuecs_query_table_destroy_query(
    nuecs_query_table_t *table,
    nuecs_query_data_t *query
);

nu_result_t nuecs_query_table_notify_new_archetype(
    nuecs_query_table_t *table,
    nuecs_archetype_data_t *archetype
);
nu_result_t nuecs_query_table_notify_new_chunk(
    nuecs_query_table_t *table,
    nuecs_chunk_data_t *chunk
);

#endif