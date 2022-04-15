#ifndef NUECS_QUERY_H
#define NUECS_QUERY_H

#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/chunk.h>

typedef struct {
    nu_array_t chunk_views;
    nu_array_t chunk_references;
    nu_array_t archetype_slots;
    uint32_t include_component_count;
    uint32_t *include_component_ids;
    uint32_t exclude_component_count;
    uint32_t *exclude_component_ids;
    uint32_t id;
} nuecs_query_data_t;

nu_result_t nuecs_query_create(nuecs_scene_data_t *scene, const nuecs_query_info_t *info, nuecs_query_t *handle);
nu_result_t nuecs_query_destroy(nuecs_scene_data_t *scene, nuecs_query_t handle);
nu_result_t nuecs_query_resolve(nuecs_scene_data_t *scene, nuecs_query_data_t *query, nuecs_query_result_t *result);
nu_result_t nuecs_query_initialize(
    nuecs_query_data_t *query, 
    uint32_t *include_components, 
    uint32_t include_component_count,
    uint32_t *exclude_components,
    uint32_t exclude_component_count
);
nu_result_t nuecs_query_terminate(nuecs_query_data_t *query);
nu_result_t nuecs_query_notify_new_chunk(nuecs_query_data_t *query, nuecs_chunk_data_t *chunk);
nu_result_t nuecs_query_try_subscribe(nuecs_query_data_t *query, nuecs_archetype_slot_t *slot);

#endif