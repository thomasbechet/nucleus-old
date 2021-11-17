#ifndef NUECS_CHUNK_H
#define NUECS_CHUNK_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_chunk_create(nuecs_chunk_data_t *chunk, uint32_t total_data_size, uint32_t entity_count);
nu_result_t nuecs_chunk_destroy(nuecs_chunk_data_t *chunk);
nu_result_t nuecs_chunk_add(
    nuecs_chunk_data_t *chunk, 
    const uint32_t *offsets, 
    const uint32_t *sizes,
    nuecs_component_data_ptr_t *data,
    uint32_t type_count, 
    uint32_t *id
);
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    const uint32_t *offsets,
    const uint32_t *sizes,
    uint32_t type_count,
    uint32_t id
);

#endif