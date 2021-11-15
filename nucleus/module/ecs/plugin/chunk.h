#ifndef NUECS_CHUNK_H
#define NUECS_CHUNK_H

#include <nucleus/module/ecs/plugin/types.h>

nu_result_t nuecs_chunk_create(nuecs_chunk_data_t *chunk, uint32_t data_size, uint32_t entity_count);
nu_result_t nuecs_chunk_destroy(nuecs_chunk_data_t *chunk);

#endif