#ifndef NUECS_CHUNK_H
#define NUECS_CHUNK_H

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/types_public.h>

#define NUECS_CHUNK_SIZE 512

typedef struct {
    nuecs_archetype_data_t *archetype;
    uint32_t size;
    uint32_t frame_size;
    uint32_t free_count;
    uint32_t *indice_table;
    nuecs_component_data_ptr_t *component_list_ptrs;
} nuecs_chunk_data_t;

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
nu_result_t nuecs_chunk_transfer(
    nuecs_chunk_data_t *chunk,
    nuecs_chunk_data_t *dst,
    uint32_t id,
    uint32_t *dst_id
);
nu_result_t nuecs_chunk_get_component(
    nuecs_chunk_data_t *chunk,
    uint32_t id,
    uint32_t component_index,
    nuecs_component_data_ptr_t *component
);
nu_result_t nuecs_chunk_write_component(
    nuecs_chunk_data_t *chunk,
    uint32_t id,
    uint32_t component_index,
    nuecs_component_data_ptr_t data
);

#endif