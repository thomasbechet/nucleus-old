#include <nucleus/module/ecs/plugin/query_table.h>

#include <nucleus/module/ecs/plugin/query.h>
#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/chunk_table.h>

static nu_result_t nuecs_query_archetype_slot_initialize(nuecs_query_archetype_slot_t *slot, nuecs_archetype_data_t *archetype)
{
    slot->archetype = archetype;
    nu_array_allocate(&slot->query_references, sizeof(nuecs_query_data_t*));
    return NU_SUCCESS;
}
static nu_result_t nuecs_query_archetype_slot_terminate(nuecs_query_archetype_slot_t *slot)
{
    nu_array_free(slot->query_references);
    return NU_SUCCESS;
}
static bool nuecs_query_reference_equals(const void *user, const void *object)
{
    return *((nuecs_query_data_t**)user) == *((nuecs_query_data_t**)object);
}
static bool nuecs_query_match_archetype(
    nuecs_query_data_t *query,
    nuecs_archetype_data_t *archetype
)
{
    /* check include all components */
    bool include_all = true;
    for (uint32_t i = 0; i < query->include_component_count; i++) {
        bool found = false;
        for (uint32_t j = 0; j < archetype->component_count; j++) {
            if (archetype->component_ids[j] == query->include_component_ids[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            include_all = false;
            break;
        }
    }

    /* check exclude all components */
    bool exclude_all = true;
    for (uint32_t i = 0; i < query->exclude_component_count; i++) {
        bool found = false;
        for (uint32_t j = 0; j < archetype->component_count; j++) {
            if (archetype->component_ids[j] == query->exclude_component_ids[j]) {
                found = true;
                break;
            }
        }
        if (found) {
            exclude_all = false;
            break;
        }
    }

    return (include_all && exclude_all);
}

nu_result_t nuecs_query_table_initialize(nuecs_query_table_t *table)
{
    nu_array_allocate(&table->queries, sizeof(nuecs_query_data_t*));
    nu_array_allocate(&table->archetype_slots, sizeof(nuecs_query_archetype_slot_t));
    return NU_SUCCESS;
}
nu_result_t nuecs_query_table_terminate(nuecs_query_table_t *table)
{
    /* slots */
    nuecs_query_archetype_slot_t *slots; uint32_t slot_count;
    nu_array_get_data(table->archetype_slots, &slots, &slot_count);
    for (uint32_t i = 0; i < slot_count; i++) {
        if (slots[i].archetype) {
            nuecs_query_archetype_slot_terminate(&slots[i]);
        }
    }
    nu_array_free(table->archetype_slots);
    /* queries */
    nuecs_query_data_t **queries; uint32_t query_count;
    nu_array_get_data(table->queries, &queries, &query_count);
    for (uint32_t i = 0; i < query_count; i++) {
        nuecs_query_terminate(queries[i]);
        nu_free(queries[i]);
    }
    nu_array_free(table->queries);
    return NU_SUCCESS;
}

nu_result_t nuecs_query_table_create_query(
    nuecs_query_table_t *table,
    nuecs_chunk_table_t *chunk_table,
    uint32_t *include_components, 
    uint32_t include_component_count,
    uint32_t *exclude_components,
    uint32_t exclude_component_count,
    nuecs_query_data_t **query
)
{
    /* allocate query */
    nuecs_query_data_t *new = (nuecs_query_data_t*)nu_malloc(sizeof(nuecs_query_data_t));
    new->id = nu_array_get_size(table->queries);
    nuecs_query_initialize(new, include_components, include_component_count, exclude_components, exclude_component_count);
    nu_array_push(table->queries, &new);

    /* check existing archetype to subscribe */
    nuecs_query_archetype_slot_t *slots; uint32_t slot_count;
    nu_array_get_data(table->archetype_slots, &slots, &slot_count);
    for (uint32_t i = 0; i < slot_count; i++) {
        /* check match */
        if (nuecs_query_match_archetype(new, slots[i].archetype)) {
            /* subscribe */
            nu_array_push(slots[i].query_references, new);
            nu_array_push(new->archetype_slots, &i);
            /* notify existing chunks */
            nuecs_chunk_archetype_slot_t *chunk_slot;
            nu_array_get(chunk_table->archetype_slots, i, &chunk_slot);
            nuecs_chunk_data_t **chunks; uint32_t chunk_count;
            nu_array_get_data(chunk_slot->chunks, &chunks, &chunk_count);
            for (uint32_t j = 0; j < chunk_count; j++) {
                /* add new chunk */
                nuecs_query_add_new_chunk(new, chunks[j]);
            }
        }
    }

    /* return */
    *query = new;

    return NU_SUCCESS;
}
nu_result_t nuecs_query_table_destroy_query(
    nuecs_query_table_t *table,
    nuecs_query_data_t *query
)
{
    /* TODO: */
    return NU_SUCCESS;
}

nu_result_t nuecs_query_table_notify_new_archetype(
    nuecs_query_table_t *table,
    nuecs_archetype_data_t *archetype
)
{
    /* add new slot */
    NU_ASSERT(nu_array_get_size(table->archetype_slots) == archetype->archetype_slot);
    nu_array_push(table->archetype_slots, NULL);
    nuecs_query_archetype_slot_t *slot;
    nu_array_get_last(table->archetype_slots, &slot);

    /* initialize slot */
    nuecs_query_archetype_slot_initialize(slot, archetype);

    /* try subscribe existing queries */
    nuecs_query_data_t **queries; uint32_t query_count;
    nu_array_get_data(table->queries, &queries, &query_count);
    for (uint32_t i = 0; i < query_count; i++) {
        /* check subscribe */
        if (nuecs_query_match_archetype(queries[i], archetype)) {
            /* subscribe */
            nu_array_push(slot->query_references, &queries[i]);
            nu_array_push(queries[i]->archetype_slots, &archetype->archetype_slot);
        }
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_query_table_notify_new_chunk(
    nuecs_query_table_t *table,
    nuecs_chunk_data_t *chunk
)
{
    /* get slot */
    NU_ASSERT(chunk->archetype->archetype_slot < nu_array_get_size(table->archetype_slots));
    nuecs_query_archetype_slot_t *slot;
    nu_array_get(table->archetype_slots, chunk->archetype->archetype_slot, &slot);

    /* iterate over queries */
    nuecs_query_data_t **queries; uint32_t query_count;
    nu_array_get_data(slot->query_references, &queries, &query_count);
    for (uint32_t i = 0; i < query_count; i++) {
        /* notify new chunk */
        nuecs_query_add_new_chunk(queries[i], chunk);
    }

    return NU_SUCCESS;
}