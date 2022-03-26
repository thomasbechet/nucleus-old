#include <nucleus/module/ecs/plugin/entity.h>

#include <nucleus/module/ecs/plugin/scene.h>
#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/utility.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/archetype_table.h>

static uint32_t nuecs_next_version(uint32_t a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

nu_result_t nuecs_entity_create(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    const nuecs_entity_info_t* info,
    nuecs_entity_t* handle
)
{
    /* sanatize components */
    nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;
    nuecs_sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

    /* find the index */
    uint32_t index;
    if (!nu_array_is_empty(scene->free_indices)) {
        uint32_t *pdata;
        nu_array_get_last(scene->free_indices, &pdata);
        index = *pdata;
        nu_array_free(scene->free_indices);
    } else {
        nu_array_push(scene->entities, NULL);
        index = nu_array_get_size(scene->entities) - 1;
    }
    nuecs_entity_data_t *entity;
    nu_array_get(scene->entities, index, &entity);

    /* find archetype */
    nuecs_archetype_data_t *archetype;
    nuecs_component_manager_find_archetype(manager, components, component_count, &archetype);

    /* find chunk */
    nuecs_chunk_data_t *chunk;
    nuecs_archetype_table_get_next_chunk(scene->archetype_table, scene->queries, archetype, &chunk);

    /* add to chunk */
    uint32_t id;
    nuecs_chunk_add(chunk, &id);
    entity->chunk    = chunk;
    entity->chunk_id = id;

    /* generate version */
    entity->version = (uint8_t)nuecs_next_version((uint32_t)entity->version);

    /* copy components */
    for (uint32_t i = 0; i < component_count; i++) {
        /* find component index */
        uint32_t component_index;
        nuecs_archetype_find_component_index(archetype, components[i]->id, &component_index);
        /* write component */
        nuecs_chunk_write_component(chunk, id, component_index, info->component_data[i]);
    }

    /* write handle */
    NU_HANDLE_SET_ID(*handle, NUECS_ENTITY_HANDLE_BUILD(index, entity->version));

    return NU_SUCCESS;
}
nu_result_t nuecs_entity_destroy(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle
)
{
    /* get entity index */
    uint32_t id, index;
    NU_HANDLE_GET_ID(handle, id);
    index = NUECS_ENTITY_HANDLE_GET_INDEX(id);

    /* get entity */
    nuecs_entity_data_t *entity;
    nu_array_get(scene->entities, index, &entity);

    /* reset entity slot */ 
    entity->chunk = NULL;

    /* save free index */
    nu_array_push(scene->free_indices, &index);

    return NU_SUCCESS;
}
static bool archetype_has_component(const nuecs_archetype_data_t *archetype, const nuecs_component_data_t *component)
{
    for (uint32_t i = 0; i < archetype->component_count; i++) {
        if (component->id == archetype->component_ids[i]) {
            return true;
        }
    }
    return false;
}
nu_result_t nuecs_entity_add_component(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle, 
    nuecs_component_t component, 
    nuecs_component_data_ptr_t data
)
{
    /* get entity entry */
    uint32_t id, index;
    NU_HANDLE_GET_ID(handle, id);
    index = NUECS_ENTITY_HANDLE_GET_INDEX(id);
    nuecs_entity_data_t *entity;
    nu_array_get(scene->entities, index, &entity);

    /* get component data */
    nuecs_component_data_t *component_data = (nuecs_component_data_t*)component;

    /* check entity has component */
    if (archetype_has_component(entity->chunk->archetype, component_data)) return NU_FAILURE;

    /* find next archetype */
    nuecs_archetype_data_t *next_archetype;
    nuecs_archetype_find_next(entity->chunk->archetype, component_data, &next_archetype);

    /* find next chunk */
    nuecs_chunk_data_t *next_chunk;
    nuecs_archetype_table_get_next_chunk(scene->archetype_table, scene->queries, next_archetype, &next_chunk);

    /* transfer data */
    uint32_t new_id;
    nuecs_chunk_transfer(entity->chunk, next_chunk, entity->chunk_id, &new_id);

    /* write new component */
    uint32_t component_index;
    nuecs_archetype_find_component_index(next_archetype, component_data->id, &component_index);
    nuecs_chunk_write_component(next_chunk, new_id, component_index, data);

    /* update entity */
    entity->chunk    = next_chunk;
    entity->chunk_id = new_id;

    return NU_SUCCESS;
}
nu_result_t nuecs_entity_remove_component(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle, 
    nuecs_component_t component
)
{
    /* get entity entry */
    uint32_t id, index;
    NU_HANDLE_GET_ID(handle, id);
    index = NUECS_ENTITY_HANDLE_GET_INDEX(id);
    nuecs_entity_data_t *entity;
    nu_array_get(scene->entities, index, &entity);

    /* get component data */
    nuecs_component_data_t *component_data = (nuecs_component_data_t*)component;

    /* check entity has component */
    if (!archetype_has_component(entity->chunk->archetype, component_data)) return NU_FAILURE;

    /* find previous archetype */
    nuecs_archetype_data_t *previous_archetype;
    nuecs_archetype_find_previous(entity->chunk->archetype, component_data, &previous_archetype);    

    /* find previous chunk */
    nuecs_chunk_data_t *previous_chunk;
    nuecs_archetype_table_get_next_chunk(scene->archetype_table, scene->queries, previous_archetype, &previous_chunk);

    /* transfer data */
    uint32_t new_id;
    nuecs_chunk_transfer(entity->chunk, previous_chunk, entity->chunk_id, &new_id);

    /* update entity */
    entity->chunk    = previous_chunk;
    entity->chunk_id = new_id;

    return NU_SUCCESS;
}