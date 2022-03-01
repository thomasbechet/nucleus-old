#include <nucleus/module/ecs/plugin/archetype_table.h>

#include <nucleus/module/ecs/plugin/chunk.h>

nu_result_t nuecs_archetype_table_initialize(nu_array_t *table)
{
    nu_array_allocate(table, sizeof(nuecs_archetype_chunks_data_t));
    return NU_SUCCESS;
}
nu_result_t nuecs_archetype_table_terminate(nu_array_t table)
{
    nuecs_archetype_chunks_data_t *data = (nuecs_archetype_chunks_data_t*)nu_array_get_data(table);
    uint32_t size                       = nu_array_get_size(table);
    for (uint32_t i = 0; i < size; i++) {
        if (data[i].archetype) {
            nuecs_chunk_data_t **chunks = (nuecs_chunk_data_t**)nu_array_get_data(data[i].chunks);
            uint32_t chunk_count        = nu_array_get_size(data[i].chunks);
            for (uint32_t j = 0; j < chunk_count; j++) {
                nuecs_chunk_free(chunks[j]);
            }
            nu_array_free(data[i].chunks);
        }
    }
    nu_array_free(table);
    return NU_SUCCESS;
}
static bool find_chunk_not_full(const void *user, const void *object) {
    (void)user;
    nuecs_chunk_data_t *data = *(nuecs_chunk_data_t**)object;
    return data->size < NUECS_CHUNK_SIZE;
}
nu_result_t nuecs_archetype_table_get_chunk(
    nu_array_t table, 
    nuecs_archetype_data_t *archetype, 
    nuecs_chunk_data_t **output
)
{
    /* resize table if needed */
    uint32_t table_size = nu_array_get_size(table);
    if (archetype->index >= table_size) {
        /* resize table */
        uint32_t new_size = archetype->index + 1;
        nu_array_resize(table, new_size);
        /* initialize new items */
        nuecs_archetype_chunks_data_t *data = (nuecs_archetype_chunks_data_t*)nu_array_get_data(table);
        for (uint32_t i = table_size; i < new_size; i++) {
            data[i].archetype = NULL;
        }
    }

    /* get the archetype chunks */
    nuecs_archetype_chunks_data_t *row = (nuecs_archetype_chunks_data_t*)nu_array_get(table, archetype->index);
    
    /* create new archetype chunks */
    if (row->archetype == NULL) {
        row->archetype = archetype;
        nu_array_allocate(&row->chunks, sizeof(nuecs_chunk_data_t*));
    }

    /* find free chunk */
    nuecs_chunk_data_t *chunk = NULL;
    uint32_t chunk_index;
    if (nu_array_find_index(row->chunks, find_chunk_not_full, NULL, &chunk_index)) {
        chunk = *(nuecs_chunk_data_t**)nu_array_get(row->chunks, chunk_index);
    } else {
        /* create new chunk */
        nuecs_chunk_allocate(archetype, &chunk);
        nu_array_push(row->chunks, &chunk);
    }

    /* return value */
    *output = chunk;

    return NU_SUCCESS;
}