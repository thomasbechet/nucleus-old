#include <nucleus/module/ecs/plugin/query.h>

#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/chunk_table.h>
#include <nucleus/module/ecs/plugin/utility.h>

static bool query_reference_equals(const void *user, const void *object)
{
    return *((nuecs_query_data_t**)user) == *((nuecs_query_data_t**)object);
}

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

    /* allocate query */
    nuecs_query_data_t *query = (nuecs_query_data_t*)nu_malloc(sizeof(nuecs_query_data_t));
    nu_indexed_array_add(scene->queries, &query, &query->id);
    nuecs_query_initialize(query, include_components, include_component_count, exclude_components, exclude_component_count);

    /* try to subscribe to archetype slots */
    nuecs_archetype_slot_t *slots; uint32_t slot_count;
    nu_array_get_data(scene->chunk_table.slots, &slots, &slot_count);
    for (uint32_t i = 0; i < slot_count; i++) {
        if (slots[i].archetype) {
            nuecs_query_try_subscribe(query, &slots[i]);
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
    nu_array_allocate(&query->archetype_slots, sizeof(nuecs_archetype_slot_t*)); /* FIXME: INVALID !!! */
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
    /* unsubscribe */
    nuecs_archetype_slot_t **slots; uint32_t slot_count;
    nu_array_get_data(query->archetype_slots, &slots, &slot_count);
    for (uint32_t i = 0; i < slot_count; i++) {
        nu_array_remove(slots[i]->notify_queries, query_reference_equals, &query);
    }

    /* free views */
    nuecs_query_chunk_view_t *views;
    uint32_t view_count;
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
nu_result_t nuecs_query_notify_new_chunk(nuecs_query_data_t *query, nuecs_chunk_data_t *chunk)
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
nu_result_t nuecs_query_try_subscribe(nuecs_query_data_t *query, nuecs_archetype_slot_t *slot)
{
    /* get archetype */
    const nuecs_archetype_data_t *archetype = slot->archetype;

    /* check include all components */
    bool include_all = true;
    for (uint32_t i = 0; i < query->include_component_count; i++) {
        bool found = false;
        for (uint32_t j = 0; j < archetype->component_count; j++) {
            if (archetype->component_ids[j] == query->include_component_ids[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            include_all = false;
            break;
        }
    }

    /* check exclude all components */
    bool exclude_all = true;
    for (uint32_t i = 0; i < query->exclude_component_count; i++) {
        bool found = false;
        for (uint32_t j = 0; j < archetype->component_count; j++) {
            if (archetype->component_ids[j] == query->exclude_component_ids[j]) {
                found = true;
                break;
            }
        }
        if (found) {
            exclude_all = false;
            break;
        }
    }

    /* subscribe to the slot */
    if (include_all && exclude_all) {
        
        /* add references to the slot and query (both directions) */
        nu_array_push(slot->notify_queries, &query);
        nu_array_push(query->archetype_slots, &slot);

        /* create existing chunk views */
        nuecs_chunk_data_t **chunks;
        uint32_t chunk_count;
        nu_array_get_data(slot->chunks, &chunks, &chunk_count);
        for (uint32_t i = 0; i < chunk_count; i++) {
            nuecs_query_notify_new_chunk(query, chunks[i]);
        }
    }

    return NU_SUCCESS;
}
