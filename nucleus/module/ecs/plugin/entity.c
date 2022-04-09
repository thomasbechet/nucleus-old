#include <nucleus/module/ecs/plugin/entity.h>

#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/utility.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/chunk_table.h>
#include <nucleus/module/ecs/plugin/logger.h>
#include <nucleus/module/ecs/plugin/entity_reference.h>

nu_result_t nuecs_entity_create(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    const nuecs_entity_info_t* info,
    nuecs_entity_t* handle
)
{
    /* sanatize components */
    uint32_t components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;
    nuecs_sanatize_components(info->components, info->component_count, components, &component_count);

    /* add entry */
    nuecs_entity_add(manager, scene, components, component_count, handle);

    /* write components */
    if (info->component_data) {
        /* get chunk */
        nuecs_chunk_data_t **pchunk;
        nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(*handle), &pchunk);
        nuecs_chunk_data_t *chunk = *pchunk;
        /* write */
        for (uint32_t i = 0; i < component_count; i++) {
            /* find component index */
            uint32_t component_index;
            nuecs_archetype_find_component_index(chunk->archetype, components[i], &component_index);
            /* write component */
            nuecs_chunk_write_component(chunk, *handle, component_index, info->component_data[i]);
        }
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_entity_destroy(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle
)
{
    /* get chunk */
    uint32_t chunk_id = NUECS_ENTITY_CHUNK(handle);
    nuecs_chunk_data_t **pchunk;
    nu_array_get(scene->chunks, chunk_id, &pchunk);
    nuecs_chunk_data_t *chunk = *pchunk;

    /* check entity */
    if (!nuecs_chunk_check_entity(chunk, handle)) return NU_FAILURE;

    /* remove entry */
    return nuecs_chunk_remove(chunk, handle);
}
nu_result_t nuecs_entity_add(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    uint32_t *components,
    uint32_t component_count,
    nuecs_entity_t *handle
)
{
    /* find archetype */
    nuecs_archetype_data_t *archetype;
    nuecs_component_manager_find_archetype(manager, components, component_count, &archetype);

    /* find chunk */
    nuecs_chunk_data_t *chunk; bool new_chunk;
    nuecs_chunk_table_get_next_chunk(&scene->chunk_table, scene->queries, archetype, &chunk, &new_chunk);
    if (new_chunk) {
        chunk->id = nu_array_get_size(scene->chunks);
        nu_array_push(scene->chunks, &chunk);
    }

    /* add entry */
    nuecs_chunk_add(chunk, handle);

    /* initialize reference index */
    nuecs_chunk_set_entity_reference(chunk, *handle, NUECS_ENTITY_REFERENCE_NONE);

    return NU_SUCCESS;
}
nu_result_t nuecs_entity_remove(
    nuecs_scene_data_t *scene,
    nuecs_entity_t handle
)
{
    /* get chunk */
    nuecs_chunk_data_t **pchunk;
    nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(handle), &pchunk);
    nuecs_chunk_data_t *chunk = *pchunk;

    /* check version */
    if (!nuecs_chunk_check_entity(chunk, handle)) return NU_FAILURE;

    /* remove entry in chunk */
    nuecs_chunk_remove(chunk, handle);

    /* reinitialize references */
    uint32_t reference_index;
    nuecs_chunk_get_entity_reference(chunk, handle, &reference_index);
    nuecs_entity_reference_data_t *reference;
    nu_array_get(scene->references, reference_index, &reference);
    reference->version = nuecs_next_version(&scene->next_reference_version);
    nu_array_push(scene->free_references, &reference_index);

    /* reset reference index */
    nuecs_chunk_set_entity_reference(chunk, handle, NUECS_ENTITY_REFERENCE_NONE);

    return NU_SUCCESS;
}
static bool archetype_has_component(const nuecs_archetype_data_t *archetype, uint32_t component_id)
{
    for (uint32_t i = 0; i < archetype->component_count; i++) {
        if (component_id == archetype->component_ids[i]) {
            return true;
        }
    }
    return false;
}
nu_result_t nuecs_entity_add_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    nuecs_entity_t *handle, 
    nuecs_component_t component, 
    nuecs_component_data_ptr_t data
)
{
    /* get chunk */
    nuecs_chunk_data_t **pchunk;
    nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(*handle), &pchunk);
    nuecs_chunk_data_t *chunk = *pchunk;

    /* check entity */
    if (!nuecs_chunk_check_entity(chunk, *handle)) return NU_FAILURE;

    /* get component id */
    uint32_t component_id;
    NU_HANDLE_GET_ID(component, component_id);

    /* check entity has component */
    if (archetype_has_component(chunk->archetype, component_id)) return NU_FAILURE;

    /* find next archetype */
    nuecs_archetype_data_t *next_archetype;
    nuecs_component_manager_find_next_archetype(manager, chunk->archetype, component_id, &next_archetype);

    /* find next chunk */
    nuecs_chunk_data_t *next_chunk; bool new_chunk;
    nuecs_chunk_table_get_next_chunk(&scene->chunk_table, scene->queries, next_archetype, &next_chunk, &new_chunk);
    if (new_chunk) {
        next_chunk->id = nu_array_get_size(scene->chunks);
        nu_array_push(scene->chunks, &next_chunk);
    }

    /* get reference index */
    uint32_t reference_index;
    nuecs_chunk_get_entity_reference(chunk, *handle, &reference_index);
    nuecs_chunk_set_entity_reference(chunk, *handle, NUECS_ENTITY_REFERENCE_NONE);

    /* transfer entity */
    nuecs_chunk_transfer(chunk, next_chunk, handle);

    /* update reference index */
    if (reference_index != NUECS_ENTITY_REFERENCE_NONE) {
        nuecs_entity_reference_data_t *reference;
        nu_array_get(scene->references, reference_index, &reference);
        reference->entity = *handle;
        nuecs_chunk_set_entity_reference(next_chunk, *handle, reference_index);
    }

    /* write new component */
    uint32_t component_index;
    nuecs_archetype_find_component_index(next_archetype, component_id, &component_index);
    nuecs_chunk_write_component(next_chunk, *handle, component_index, data);

    return NU_SUCCESS;
}
nu_result_t nuecs_entity_remove_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    nuecs_entity_t *handle,
    nuecs_component_t component
)
{
    /* get chunk */
    nuecs_chunk_data_t **pchunk;
    nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(*handle), &pchunk);
    nuecs_chunk_data_t *chunk = *pchunk;

    /* check entity */
    if (!nuecs_chunk_check_entity(chunk, *handle)) return NU_FAILURE;

    /* get component id */
    uint32_t component_id;
    NU_HANDLE_GET_ID(component, component_id);

    /* check entity has component */
    if (!archetype_has_component(chunk->archetype, component_id)) return NU_FAILURE;

    /* find previous archetype */
    nuecs_archetype_data_t *previous_archetype;
    nuecs_component_manager_find_previous_archetype(manager, chunk->archetype, component_id, &previous_archetype);

    /* find previous chunk */
    nuecs_chunk_data_t *previous_chunk; bool new_chunk;
    nuecs_chunk_table_get_next_chunk(&scene->chunk_table, scene->queries, previous_archetype, &previous_chunk, &new_chunk);
    if (new_chunk) {
        previous_chunk->id = nu_array_get_size(scene->chunks);
        nu_array_push(scene->chunks, &previous_chunk);
    }

    /* get reference index and reset */
    uint32_t reference_index;
    nuecs_chunk_get_entity_reference(chunk, *handle, &reference_index);
    nuecs_chunk_set_entity_reference(chunk, *handle, NUECS_ENTITY_REFERENCE_NONE);

    /* transfer data */
    nuecs_chunk_transfer(chunk, previous_chunk, handle);

    /* update reference index */
    if (reference_index != NUECS_ENTITY_REFERENCE_NONE) {
        nuecs_entity_reference_data_t *reference;
        nu_array_get(scene->references, reference_index, &reference);
        reference->entity = *handle;
        nuecs_chunk_set_entity_reference(previous_chunk, *handle, reference_index);
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_entity_get_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    nuecs_entity_t handle,
    nuecs_component_t component,
    nuecs_component_data_ptr_t *data
)
{
    /* get chunk */
    nuecs_chunk_data_t **pchunk;
    nu_array_get(scene->chunks, NUECS_ENTITY_CHUNK(handle), &pchunk);
    nuecs_chunk_data_t *chunk = *pchunk;

    /* get component id */
    uint32_t component_id;
    NU_HANDLE_GET_ID(component, component_id);

    /* return component */
    uint32_t component_index;
    nuecs_archetype_find_component_index(chunk->archetype, component_id, &component_index);
    return nuecs_chunk_get_component(chunk, handle, component_index, data);
}