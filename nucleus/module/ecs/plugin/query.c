#include <nucleus/module/ecs/plugin/query.h>

#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/utility.h>

nu_result_t nuecs_query_create(nuecs_scene_data_t *scene, const nuecs_query_info_t *info, nuecs_query_t *handle)
{
    /* sanitize components */
    uint32_t include_components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t include_component_count;
    nuecs_sanatize_components(info->include_components, info->include_component_count, 
        include_components, &include_component_count);
    uint32_t exclude_components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t exclude_component_count;
    nuecs_sanatize_components(info->exclude_components, info->exclude_component_count, 
        exclude_components, &exclude_component_count);

    /* create query */
    nuecs_query_data_t *query;
    nuecs_query_table_create_query(&scene->query_table, &scene->chunk_table, 
        include_components, include_component_count, exclude_components, exclude_component_count, &query);

    /* save handle */
    *handle = (nuecs_query_t)query;

    return NU_SUCCESS;
}
nu_result_t nuecs_query_destroy(nuecs_scene_data_t *scene, nuecs_query_t handle)
{
    /* recover handles */
    nuecs_query_data_t *query = (nuecs_query_data_t*)handle;
    nuecs_query_table_destroy_query(&scene->query_table, query);
    return NU_SUCCESS;
}
nu_result_t nuecs_query_resolve(nuecs_scene_data_t *scene, nuecs_query_data_t *query, nuecs_query_result_t *result)
{
    (void)scene;
    /* update chunks */
    nuecs_query_chunk_view_t *views_data;
    uint32_t view_count;
    nu_array_get_data(query->chunk_views, &views_data, &view_count);
    nuecs_chunk_data_t **chunks_data;
    nu_array_get_data(query->chunk_references, &chunks_data, NULL);
    
    /* update views count */
    for (uint32_t i = 0; i < view_count; i++) {
        views_data[i].count = chunks_data[i]->size;
    }

    /* return views */
    result->chunks = views_data;
    result->count  = view_count;

    return NU_SUCCESS;
}
nu_result_t nuecs_query_initialize(
    nuecs_query_data_t *query, 
    uint32_t *include_components, 
    uint32_t include_component_count,
    uint32_t *exclude_components,
    uint32_t exclude_component_count
)
{
    /* allocate memory */
    nu_array_allocate(&query->archetype_slots, sizeof(uint32_t));
    nu_array_allocate(&query->chunk_views, sizeof(nuecs_query_chunk_view_t));
    nu_array_allocate(&query->chunk_references, sizeof(nuecs_chunk_data_t*));

    /* copy component ids */
    query->include_component_count = include_component_count;
    query->include_component_ids   = (uint32_t*)nu_malloc(sizeof(include_component_count) * sizeof(uint32_t));
    for (uint32_t i = 0; i < include_component_count; i++) {
        query->include_component_ids[i] = include_components[i];
    }
    query->exclude_component_count = exclude_component_count;
    query->exclude_component_ids   = (uint32_t*)nu_malloc(sizeof(exclude_component_count) * sizeof(uint32_t));
    for (uint32_t i = 0; i < exclude_component_count; i++) {
        query->exclude_component_ids[i] = exclude_components[i];
    }
    
    return NU_SUCCESS;
}
nu_result_t nuecs_query_terminate(nuecs_query_data_t *query)
{
    /* free views */
    nuecs_query_chunk_view_t *views; uint32_t view_count;
    nu_array_get_data(query->chunk_views, &views, &view_count);
    for (uint32_t i = 0; i < view_count; i++) {
        nu_free(views[i].components);
    }

    /* free resources */
    nu_free(query->include_component_ids);
    nu_free(query->exclude_component_ids);
    nu_array_free(query->archetype_slots);
    nu_array_free(query->chunk_views);
    nu_array_free(query->chunk_references);

    return NU_SUCCESS;
}
nu_result_t nuecs_query_add_new_chunk(nuecs_query_data_t *query, nuecs_chunk_data_t *chunk)
{
    /* create new view */
    nu_array_push(query->chunk_views, NULL);
    nu_array_push(query->chunk_references, &chunk);

    /* recover allocated view */
    nuecs_query_chunk_view_t *view; nu_array_get_last(query->chunk_views, &view);
    view->components = (nuecs_component_data_ptr_t*)nu_malloc(sizeof(nuecs_component_data_ptr_t) * query->include_component_count);

    /* setup component data ptrs */
    for (uint32_t i = 0; i < query->include_component_count; i++) {
        for (uint32_t j = 0; j < chunk->archetype->component_count; j++) {
            if (query->include_component_ids[i] == chunk->archetype->component_ids[j]) {
                view->components[i] = chunk->component_list_ptrs[j];
                break;
            }
        }
    }
 
    return NU_SUCCESS;
}