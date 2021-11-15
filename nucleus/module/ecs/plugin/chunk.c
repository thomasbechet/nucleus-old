#include <nucleus/module/ecs/plugin/chunk.h>

nu_result_t nuecs_chunk_create(nuecs_chunk_data_t *chunk, uint32_t data_size, uint32_t entity_count)
{
    chunk->data       = (uint8_t*)nu_malloc(data_size);
    chunk->indices    = (uint32_t*)nu_malloc(sizeof(uint32_t) * entity_count);
    chunk->ids        = (uint32_t*)nu_malloc(sizeof(uint32_t) * entity_count);
    chunk->free_count = 0;
}
nu_result_t nuecs_chunk_destroy(nuecs_chunk_data_t *chunk)
{
    nu_free(chunk->data);
    nu_free(chunk->indices);
    nu_free(chunk->ids);

    return NU_SUCCESS;
}