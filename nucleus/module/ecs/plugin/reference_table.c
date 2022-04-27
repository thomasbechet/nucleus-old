#include <nucleus/module/ecs/plugin/reference_table.h>

#include <nucleus/module/ecs/plugin/utility.h>

nu_result_t nuecs_reference_table_initialize(nuecs_reference_table_t *table)
{
    nu_array_allocate(&table->entity_references, sizeof(nuecs_entity_reference_data_t));
    nu_array_allocate(&table->free_entity_references, sizeof(uint32_t));
    table->next_reference_version = 0;
    return NU_SUCCESS;
}
nu_result_t nuecs_reference_table_terminate(nuecs_reference_table_t *table)
{
    nu_array_free(table->entity_references);
    nu_array_free(table->free_entity_references);
    return NU_SUCCESS;
}

nu_result_t nuecs_reference_table_get_entity_reference(
    nuecs_reference_table_t *table, 
    uint32_t index,
    uint8_t version,
    nuecs_entity_t *entity
)
{
    nuecs_entity_reference_data_t *reference;
    nu_array_get(table->entity_references, index, &reference);
    if (reference->version == version) {
        *entity = reference->entity;
        return NU_SUCCESS;
    } else {
        *entity = NU_NULL_HANDLE;
        return NU_FAILURE;
    }
}
nu_result_t nuecs_reference_table_set_entity_reference(
    nuecs_reference_table_t *table,
    uint32_t index,
    nuecs_entity_t entity
)
{
    nuecs_entity_reference_data_t *reference;
    nu_array_get(table->entity_references, index, &reference);
    reference->entity = entity;
    return NU_SUCCESS;
}
nu_result_t nuecs_reference_table_add_entity_reference(
    nuecs_reference_table_t *table,
    nuecs_entity_t entity,
    uint32_t *index,
    uint8_t *version
)
{
    /* get next index */
    uint32_t reference_index;
    if (!nu_array_is_empty(table->free_entity_references)) {
        uint32_t *pdata;
        nu_array_get_last(table->free_entity_references, &pdata);
        reference_index = *pdata;
        nu_array_pop(table->free_entity_references);
    } else {
        reference_index = nu_array_get_size(table->entity_references);
        nu_array_push(table->entity_references, NULL);
    }

    /* write info */
    nuecs_entity_reference_data_t *reference;
    nu_array_get(table->entity_references, reference_index, &reference);
    reference->entity  = entity;
    reference->version = nuecs_next_version(&table->next_reference_version);

    /* return info */
    *version = reference->version;
    *index   = reference_index;

    return NU_SUCCESS;
}
nu_result_t nuecs_reference_table_remove_entity_reference(
    nuecs_reference_table_t *table,
    uint32_t index
)
{
    nuecs_entity_reference_data_t *reference;
    nu_array_get(table->entity_references, index, &reference);
    /* change version */
    reference->version = nuecs_next_version(&table->next_reference_version);
    /* add to free index */
    nu_array_push(table->free_entity_references, &index);
    return NU_SUCCESS;
}