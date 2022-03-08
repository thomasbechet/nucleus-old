#include <nucleus/module/ecs/plugin/query.h>

#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/archetype_table.h>
#include <nucleus/module/ecs/plugin/utility.h>

static nu_result_t nuecs_query_subscribe_archetypes(nuecs_query_data_t *query, nu_array_t table)
{


    return NU_SUCCESS;
}
static nu_result_t nuecs_query_create_chunk_view(nuecs_query_data_t *query, const nuecs_chunk_data_t *chunk)
{
    /* create new view */
    nu_array_push(query->chunk_views, NULL);
    nuecs_query_chunk_view_t *view = (nuecs_query_chunk_view_t*)nu_array_get_last(query->chunk_views);
    view->components = (nuecs_component_data_ptr_t*)nu_malloc(sizeof(nuecs_component_data_ptr_t) * query->component_count);

    /* setup component data ptrs */
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
    nu_array_allocate(&query->archetype_entries, sizeof(nuecs_archetype_entry_data_t*));
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
    nuecs_query_chunk_view_t *views = (nuecs_query_chunk_view_t*)nu_array_get_data(query->chunk_views);
    uint32_t view_count = nu_array_get_size(query->chunk_views);
    for (uint32_t i = 0; i < view_count; i++) {
        nu_free(views[i].components);
    }

    /* free resources */
    nu_free(query->component_ids);
    nu_array_free(query->archetype_entries);
    nu_array_free(query->chunk_views);

    return NU_SUCCESS;
}

nu_result_t nuecs_query_create(nuecs_scene_t scene_handle, const nuecs_query_info_t *info, nuecs_query_t *handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    /* sanitize components */
    nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;
    nuecs_sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

    // /* find archetype */
    // nuecs_archetype_data_t *archetype;
    // nuecs_component_manager_find_archetype(components, component_count, &archetype);

    // /* find archetype entry */
    // nuecs_archetype_entry_data_t *entry;
    // nuecs_archetype_table_get_entry(scene->archetype_table, archetype, &entry);

    /* allocate query */
    nuecs_query_data_t *query = (nuecs_query_data_t*)nu_malloc(sizeof(nuecs_query_data_t));
    nu_indexed_array_add(scene->queries, &query, &query->id);
    nuecs_query_initialize(query, info);

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
    // nuecs_query_chunk_view_t *data = (nuecs_query_chunk_view_t*)nu_array_get_data(query_data->chunk_views);
    // uint32_t chunk_count = nu_array_get_size(query_data->chunk_views);

    // query_data->

    // for (uint32_t i = 0; i < chunk_count; i++) {

    // }

    /* return views */
    chunks->count = 0;
    chunks->views = NULL;

    return NU_SUCCESS;
}
