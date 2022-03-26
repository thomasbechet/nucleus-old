#include <nucleus/module/ecs/plugin/query.h>

#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/archetype_table.h>
#include <nucleus/module/ecs/plugin/utility.h>

static bool query_reference_equals(const void *user, const void *object)
{
    return *((nuecs_query_data_t**)user) == *((nuecs_query_data_t**)object);
}

nu_result_t nuecs_query_create(nuecs_scene_data_t *scene, const nuecs_query_info_t *info, nuecs_query_t *handle)
{
    /* sanitize components */
    nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;
    nuecs_sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

    /* allocate query */
    nuecs_query_data_t *query = (nuecs_query_data_t*)nu_malloc(sizeof(nuecs_query_data_t));
    nu_indexed_array_add(scene->queries, &query, &query->id);
    nuecs_query_initialize(query, info);

    /* try to subscribe to archetype entries */
    nuecs_archetype_entry_data_t *entries;
    uint32_t entry_count;
    nu_array_get_data(scene->archetype_table, &entries, &entry_count);
    for (uint32_t i = 0; i < entry_count; i++) {
        if (entries[i].archetype) {
            nuecs_query_try_subscribe(query, &entries[i]);
        }
    }

    /* save handle */
    *handle = (nuecs_query_t)query;

    return NU_SUCCESS;
}
nu_result_t nuecs_query_destroy(nuecs_scene_data_t *scene, nuecs_query_t handle)
{
    /* recover handles */
    nuecs_query_data_t *query = (nuecs_query_data_t*)handle;

    /* remove and terminate query */
    nu_indexed_array_remove(scene->queries, query->id);
    nuecs_query_terminate(query);
    nu_free(query);

    return NU_SUCCESS;
}
nu_result_t nuecs_query_initialize(nuecs_query_data_t *query, const nuecs_query_info_t *info)
{
    /* allocate memory */
    nu_array_allocate(&query->archetype_entries, sizeof(nuecs_archetype_entry_data_t*));
    nu_array_allocate(&query->chunk_views, sizeof(nuecs_query_chunk_view_t));
    nu_array_allocate(&query->chunk_references, sizeof(nuecs_chunk_data_t*));

    /* copy component ids */
    query->component_count = info->component_count;
    query->component_ids = (uint32_t*)nu_malloc(sizeof(info->component_count) * sizeof(uint32_t));
    for (uint32_t i = 0; i < info->component_count; i++) {
        query->component_ids[i] = ((nuecs_component_data_t*)info->components[i])->id;
    }
    
    return NU_SUCCESS;
}
nu_result_t nuecs_query_terminate(nuecs_query_data_t *query)
{
    /* unsubscribe */
    nuecs_archetype_entry_data_t **entries;
    uint32_t entry_count;
    nu_array_get_data(query->archetype_entries, &entries, &entry_count);
    for (uint32_t i = 0; i < entry_count; i++) {
        nu_array_remove(entries[i]->notify_queries, query_reference_equals, &query);
    }

    /* free views */
    nuecs_query_chunk_view_t *views;
    uint32_t view_count;
    nu_array_get_data(query->chunk_views, &views, &view_count);
    for (uint32_t i = 0; i < view_count; i++) {
        nu_free(views[i].components);
    }

    /* free resources */
    nu_free(query->component_ids);
    nu_array_free(query->archetype_entries);
    nu_array_free(query->chunk_views);
    nu_array_free(query->chunk_references);

    return NU_SUCCESS;
}
nu_result_t nuecs_query_notify_new_chunk(nuecs_query_data_t *query, nuecs_chunk_data_t *chunk)
{
    /* create new view */
    nu_array_push(query->chunk_views, NULL);
    nu_array_push(query->chunk_references, &chunk);

    /* recover allocated view */
    nuecs_query_chunk_view_t *view; nu_array_get_last(query->chunk_views, &view);
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
nu_result_t nuecs_query_try_subscribe(nuecs_query_data_t *query, nuecs_archetype_entry_data_t *entry)
{
    /* add the query to the notify list */
    const nuecs_archetype_data_t *archetype = entry->archetype;
    if (nuecs_is_subset(query->component_ids, query->component_count, archetype->component_ids, archetype->component_count)) {
        
        /* add references to entry and query (both directions) */
        nu_array_push(entry->notify_queries, &query);
        nu_array_push(query->archetype_entries, &entry);

        /* create existing chunk views */
        nuecs_chunk_data_t **chunks;
        uint32_t chunk_count;
        nu_array_get_data(entry->chunks, &chunks, &chunk_count);
        for (uint32_t i = 0; i < chunk_count; i++) {
            nuecs_query_notify_new_chunk(query, chunks[i]);
        }
    }

    return NU_SUCCESS;
}

nu_result_t nuecs_query_resolve_chunks(nuecs_query_data_t *query, nuecs_query_chunks_t *chunks)
{
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
    chunks->views      = views_data;
    chunks->view_count = view_count;

    return NU_SUCCESS;
}
