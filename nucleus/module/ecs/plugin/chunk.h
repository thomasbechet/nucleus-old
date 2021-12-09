#ifndef NUECS_CHUNK_H
#define NUECS_CHUNK_H

#include <nucleus/module/ecs/plugin/types.h>

#define NUECS_CHUNK_SIZE 512

nu_result_t nuecs_chunk_allocate(nuecs_archetype_t *archetype, nuecs_chunk_data_t **chunk);
nu_result_t nuecs_chunk_free(nuecs_chunk_data_t *chunk);
nu_result_t nuecs_chunk_add(
    nuecs_chunk_data_t *chunk,
    const nuecs_component_data_ptr_t *data,
    uint32_t *id
);
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    uint32_t id
);

#endif