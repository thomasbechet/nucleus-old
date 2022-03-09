#ifndef NUECS_QUERY_H
#define NUECS_QUERY_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_query_initialize(nuecs_query_data_t *query, const nuecs_query_info_t *info);
nu_result_t nuecs_query_terminate(nuecs_query_data_t *query);
nu_result_t nuecs_query_notify_new_chunk(nuecs_query_data_t *query, nuecs_chunk_data_t *chunk);
nu_result_t nuecs_query_try_subscribe(nuecs_query_data_t *query, nuecs_archetype_entry_data_t *entry);

nu_result_t nuecs_query_create(nuecs_scene_t scene, const nuecs_query_info_t *info, nuecs_query_t *handle);
nu_result_t nuecs_query_destroy(nuecs_scene_t scene, nuecs_query_t handle);
nu_result_t nuecs_query_resolve_chunks(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_chunks_t *chunks);

#endif