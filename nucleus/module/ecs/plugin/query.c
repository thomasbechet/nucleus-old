#include <nucleus/module/ecs/plugin/query.h>

#include <nucleus/module/ecs/plugin/component_manager.h>

static nu_result_t nuecs_query_create_chunk_view(nuecs_query_data_t *query, const nuecs_chunk_data_t *chunk)
{
    /* create new view */
    nu_array_push(query->chunk_views, NULL);
    nuecs_query_chunk_view_t *view = (nuecs_query_chunk_view_t*)nu_array_get_last(query->chunk_views);

    /* allocate data pointers */
    view->components = (nuecs_component_data_ptr_t*)nu_malloc(sizeof(nuecs_component_data_ptr_t) * query->component_count);

    /* acquire pointers */
    for (uint32_t i = 0; i < query->component_count; i++) {
        for (uint32_t j = 0; j < chunk->archetype->component_count; j++) {
            if (query->component_ids[i] == chunk->archetype->component_ids[j]) {
                view->components[i] = chunk->component_list_ptrs[j];
                break;
            }
        }
    }
 
    return NU_SUCCESS;
}

nu_result_t nuecs_query_initialize(nuecs_query_data_t *query, const nuecs_query_info_t *info)
{
    /* allocate memory */
    nu_array_allocate(&query->archetype_indices, sizeof(uint32_t));
    nu_array_allocate(&query->chunk_views, sizeof(nuecs_query_chunk_view_t));

    /* copy component ids */
    query->component_count = info->component_count;
    query->component_ids = (uint32_t*)nu_malloc(sizeof(info->component_count) * sizeof(uint32_t));
    for (uint32_t i = 0; i < info->component_count; i++) {
        query->component_ids[i] = ((nuecs_component_data_t*)info->components)[i].id;
    }
    
    return NU_SUCCESS;
}
nu_result_t nuecs_query_terminate(nuecs_query_data_t *query)
{
    /* free views */
    nuecs_
    uint32_t view_count = nu_array_get_size(query->chunk_views);

    /* free resources */
    nu_array_free(query->archetype_indices);
    nu_array_free(query->chunk_views);

    return NU_SUCCESS;
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
