#include <nucleus/module/ecs/plugin/query.h>

#include <nucleus/module/ecs/plugin/component_manager.h>

nu_result_t nuecs_query_initialize(nuecs_query_data_t *query, const nuecs_query_info_t *info)
{
    nu_array_allocate(&query->archetype_indices, sizeof(uint32_t));
    // nu_array_allocate(&query->chunk_views, CHUNK_VIEW_SIZE(info->component_count));
}
nu_result_t nuecs_query_terminate(nuecs_query_data_t *query)
{
    nu_array_free(query->archetype_indices);
    nu_array_free(query->chunk_views);
}

nu_result_t nuecs_query_create(nuecs_scene_t scene_handle, const nuecs_query_info_t *info, nuecs_query_t *handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    /* find archetype */
    nuecs_archetype_data_t *archetype;
    nuecs_component_manager_find_archetype(info->components, info->component_count, &archetype);

    // nu_array_push(scene->)
    

    return NU_SUCCESS;
}
nu_result_t nuecs_query_destroy(nuecs_scene_t scene, nuecs_query_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_query_resolve_chunks(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_chunks_t *chunks)
{
    nuecs_query_data_t *query_data = (nuecs_query_data_t*)query;

    /* update chunks */
    nuecs_query_chunk_view_t *data = (nuecs_query_chunk_view_t*)nu_array_get_data(query_data->chunk_views);
    uint32_t chunk_count = nu_array_get_size(query_data->chunk_views);

    for (uint32_t i = 0; i < chunk_count; i++) {

    }

    /* return views */
    chunks->count = chunk_count;
    chunks->views = data;

    return NU_SUCCESS;
}
