#ifndef NUECS_REFERENCE_TABLE_H
#define NUECS_REFERENCE_TABLE_H

#include <nucleus/module/ecs/plugin/types_public.h>

typedef struct {
    nu_array_t entity_references;      /* nuecs_entity_reference_data_t */
    nu_array_t free_entity_references; /* uint32_t */
    uint8_t next_reference_version;
} nuecs_reference_table_t;

typedef struct {
    nuecs_entity_t entity;
    uint8_t version;
} nuecs_entity_reference_data_t;

nu_result_t nuecs_reference_table_initialize(nuecs_reference_table_t *table);
nu_result_t nuecs_reference_table_terminate(nuecs_reference_table_t *table);

nu_result_t nuecs_reference_table_get_entity_reference(
    nuecs_reference_table_t *table, 
    uint32_t index,
    uint8_t version,
    nuecs_entity_t *entity
);
nu_result_t nuecs_reference_table_set_entity_reference(
    nuecs_reference_table_t *table,
    uint32_t index,
    nuecs_entity_t entity
);
nu_result_t nuecs_reference_table_add_entity_reference(
    nuecs_reference_table_t *table,
    nuecs_entity_t entity,
    uint32_t *index,
    uint8_t *version
);
nu_result_t nuecs_reference_table_remove_entity_reference(
    nuecs_reference_table_t *table,
    uint32_t index
);

#endif