#include <nucleus/module/ecs/plugin/chunk_table.h>

#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/query.h>

nu_result_t nuecs_chunk_table_initialize(nuecs_chunk_table_t *table)
{
    nu_array_allocate(&table->slots, sizeof(nuecs_archetype_slot_t));
    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_table_terminate(nuecs_chunk_table_t *table)
{
    nuecs_archetype_slot_t *slots; uint32_t slot_count;
    nu_array_get_data(table->slots, &slots, &slot_count);
    for (uint32_t i = 0; i < slot_count; i++) {

        /* check if the slot is used */
        if (slots[i].archetype) {

            /* free chunks */
            nuecs_chunk_data_t **chunks; uint32_t chunk_count;
            nu_array_get_data(slots[i].chunks, &chunks, &chunk_count);
            for (uint32_t j = 0; j < chunk_count; j++) {
                nuecs_chunk_free(chunks[j]);
            }
            nu_array_free(slots[i].chunks);

            /* free query notification list */
            nu_array_free(slots[i].notify_queries);
        }
    }
    
    /* free resources */
    nu_array_free(table->slots);

    return NU_SUCCESS;
}
static nu_result_t nuecs_chunk_table_get_slot(
    nuecs_chunk_table_t *table, 
    const nuecs_archetype_data_t *archetype,
    nuecs_archetype_slot_t **slot
)
{
    /* resize table if needed */
    uint32_t slot_count = nu_array_get_size(table->slots);
    if (archetype->index >= slot_count) {

        /* resize table */
        uint32_t new_size = archetype->index + 1;
        nu_array_resize(table->slots, new_size);

        /* initialize new items */
        nuecs_archetype_slot_t *slots;
        nu_array_get_data(table->slots, &slots, NULL);
        for (uint32_t i = slot_count; i < new_size; i++) {
            slots[i].archetype = NULL;
        }
    }

    /* return the archetype slot */
    nuecs_archetype_slot_t *pslot; nu_array_get(table->slots, archetype->index, &pslot);
    *slot = pslot;

    return NU_SUCCESS;
}
static bool find_chunk_not_full(const void *user, const void *object) {
    (void)user;
    nuecs_chunk_data_t *data = *(nuecs_chunk_data_t**)object;
    return data->size < NUECS_CHUNK_SIZE;
}
nu_result_t nuecs_chunk_table_get_next_chunk(
    nuecs_chunk_table_t *table,
    nu_indexed_array_t queries,
    nuecs_archetype_data_t *archetype,
    nuecs_chunk_data_t **output,
    bool *new_chunk
)
{
    /* find archetype slot */
    nuecs_archetype_slot_t *slot;
    nuecs_chunk_table_get_slot(table, archetype, &slot);
    
    /* create new archetype slot */
    if (slot->archetype == NULL) {
        
        /* initialize new slot */
        slot->archetype = archetype;
        nu_array_allocate(&slot->chunks, sizeof(nuecs_chunk_data_t*));
        nu_array_allocate(&slot->notify_queries, sizeof(nuecs_query_data_t*));

        /* try to subscribe queries */
        nuecs_query_data_t **queries_data;
        uint32_t query_count;
        nu_indexed_array_get_data(queries, &queries_data, &query_count);
        for (uint32_t i = 0; i < query_count; i++) {
            nuecs_query_try_subscribe(queries_data[i], slot);
        }
    }

    /* find free chunk */
    nuecs_chunk_data_t *chunk = NULL;
    uint32_t chunk_index;
    if (nu_array_find_index(slot->chunks, find_chunk_not_full, NULL, &chunk_index)) {
        nuecs_chunk_data_t **pchunk; nu_array_get(slot->chunks, chunk_index, &pchunk);
        chunk = *pchunk;
        *new_chunk = false;
    } else {
        /* create new chunk */
        nuecs_chunk_allocate(archetype, &chunk);
        nu_array_push(slot->chunks, &chunk);
        *new_chunk = true;

        /* notify queries */
        nuecs_query_data_t **notify_queries;
        uint32_t notify_query_count;
        nu_array_get_data(slot->notify_queries, &notify_queries, &notify_query_count);
        for (uint32_t i = 0; i < notify_query_count; i++) {
            nuecs_query_notify_new_chunk(notify_queries[i], chunk);
        }
    }

    /* return value */
    *output = chunk;

    return NU_SUCCESS;
}