#include <nucleus/module/ecs/plugin/chunk_table.h>

#include <nucleus/module/ecs/plugin/query_table.h>
#include <nucleus/module/ecs/plugin/chunk.h>

static nu_result_t nuecs_chunk_archetype_slot_initialize(nuecs_chunk_archetype_slot_t *slot)
{
    nu_array_allocate(&slot->chunks, sizeof(nuecs_chunk_data_t*));
    return NU_SUCCESS;
}
static nu_result_t nuecs_chunk_archetype_slot_terminate(nuecs_chunk_archetype_slot_t *slot)
{
    /* chunks */
    nuecs_chunk_data_t **chunks; uint32_t chunk_count;
    nu_array_get_data(slot->chunks, &chunks, &chunk_count);
    for (uint32_t i = 0; i < chunk_count; i++) {
        nuecs_chunk_free(chunks[i]);
    }
    nu_array_free(slot->chunks);
    return NU_SUCCESS;
}

nu_result_t nuecs_chunk_table_initialize(nuecs_chunk_table_t *table)
{
    nu_array_allocate(&table->archetype_slots, sizeof(nuecs_chunk_archetype_slot_t));
    nu_array_allocate(&table->chunk_references, sizeof(nuecs_chunk_data_t*));
    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_table_terminate(nuecs_chunk_table_t *table)
{
    /* archetype slots */
    nuecs_chunk_archetype_slot_t *slots; uint32_t slot_count;
    nu_array_get_data(table->archetype_slots, &slots, &slot_count);
    for (uint32_t i = 0; i < slot_count; i++) {
        nuecs_chunk_archetype_slot_terminate(&slots[i]);
    }
    nu_array_free(table->archetype_slots);
    /* chunk references */
    nu_array_free(table->chunk_references);

    return NU_SUCCESS;
}

static bool find_chunk_not_full(const void *user, const void *object) {
    (void)user;
    nuecs_chunk_data_t *data = *(nuecs_chunk_data_t**)object;
    return data->size < NUECS_CHUNK_SIZE;
}
nu_result_t nuecs_chunk_table_get_next_chunk(
    nuecs_chunk_table_t *table,
    nuecs_query_table_t *query_table,
    nuecs_archetype_data_t *archetype,
    nuecs_chunk_data_t **output
)
{
    /* get the slot */
    nuecs_chunk_archetype_slot_t *slot;
    if (archetype->archetype_slot == NUECS_ARCHETYPE_SLOT_NONE) {
        /* save slot */
        archetype->archetype_slot = nu_array_get_size(table->archetype_slots);
        nu_array_push(table->archetype_slots, NULL);
        nu_array_get_last(table->archetype_slots, &slot);
        /* initialize */
        nuecs_chunk_archetype_slot_initialize(slot);
        /* notify query table */
        nuecs_query_table_notify_new_archetype(query_table, archetype);
    } else {
        nu_array_get(table->archetype_slots, archetype->archetype_slot, &slot);
    }

    /* find free chunk */
    nuecs_chunk_data_t *chunk = NULL;
    uint32_t chunk_index;
    if (nu_array_find_index(slot->chunks, find_chunk_not_full, NULL, &chunk_index)) {
        nuecs_chunk_data_t **pchunk; nu_array_get(slot->chunks, chunk_index, &pchunk);
        chunk = *pchunk;
    } else {
        /* create new chunk */
        nuecs_chunk_allocate(archetype, &chunk);
        nu_array_push(slot->chunks, &chunk);
        chunk->id = nu_array_get_size(table->chunk_references);
        nu_array_push(table->chunk_references, &chunk);
        /* notify query table */
        nuecs_query_table_notify_new_chunk(query_table, chunk);
    }

    /* return chunk */
    *output = chunk;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_table_get_chunk(
    nuecs_chunk_table_t *table,
    uint32_t chunk_id,
    nuecs_chunk_data_t **chunk
)
{
    nuecs_chunk_data_t **pchunk;
    nu_array_get(table->chunk_references, chunk_id, &pchunk);
    *chunk = *pchunk;
    return NU_SUCCESS;
}