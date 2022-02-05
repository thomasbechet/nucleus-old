#ifndef NUECS_CHUNK_H
#define NUECS_CHUNK_H

#include <nucleus/module/ecs/plugin/types.h>

#define NUECS_CHUNK_SIZE 512

nu_result_t nuecs_chunk_allocate(nuecs_archetype_data_t *archetype, nuecs_chunk_data_t **chunk);
nu_result_t nuecs_chunk_free(nuecs_chunk_data_t *chunk);
nu_result_t nuecs_chunk_add(
    nuecs_chunk_data_t *chunk,
    uint32_t *id
);
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    uint32_t id
);
nu_result_t nuecs_chunk_get_component(
    nuecs_chunk_data_t *chunk,
    uint32_t entity_id,
    uint32_t component_index,
    nuecs_component_data_ptr_t *component
);
nu_result_t nuecs_chunk_write_component(
    nuecs_chunk_data_t *chunk,
    uint32_t entity_id,
    uint32_t component_index,
    nuecs_component_data_ptr_t data
);

#endif