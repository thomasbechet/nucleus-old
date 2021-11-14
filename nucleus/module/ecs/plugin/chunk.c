#include <nucleus/module/ecs/plugin/chunk.h>

nu_result_t nuecs_chunk_create(nuecs_chunk_data_t *chunk, const nuecs_component_type_t **types, uint32_t type_count)
{
    // chunk->offsets = (uint32_t*)nu_malloc(sizeof(uint32_t) * type_count);
    
    // uint32_t total_size = 0;
    // for (uint32_t i = 0; i < type_count; i++) {
    //     chunk->offsets[i] = total_size;
    //     total_size       += types[i]->size * NUECS_CHUNK_SIZE;
    // }

    // chunk->free_count = 0;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_destroy(nuecs_chunk_data_t *chunk)
{


    return NU_SUCCESS;
}