#ifndef NUECS_QUERY_H
#define NUECS_QUERY_H

#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/chunk.h>

typedef struct {
    nu_array_t chunk_views;
    nu_array_t chunk_references;
    nu_array_t archetype_entries;
    uint32_t component_count;
    uint32_t *component_ids;
    uint32_t id;
} nuecs_query_data_t;

nu_result_t nuecs_query_initialize(nuecs_query_data_t *query, const nuecs_query_info_t *info);
nu_result_t nuecs_query_terminate(nuecs_query_data_t *query);
nu_result_t nuecs_query_notify_new_chunk(nuecs_query_data_t *query, nuecs_chunk_data_t *chunk);
nu_result_t nuecs_query_try_subscribe(nuecs_query_data_t *query, nuecs_archetype_entry_data_t *entry);
nu_result_t nuecs_query_resolve_chunks(nuecs_query_data_t *query, nuecs_query_chunks_t *chunks);

#endif