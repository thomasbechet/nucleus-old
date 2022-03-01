#include <nucleus/module/ecs/plugin/scene.h>

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/system.h>
#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/archetype_table.h>

static void sanatize_components(
    nuecs_component_data_t **in_components,
    uint32_t in_component_count,
    nuecs_component_data_t **components,
    uint32_t *component_count
)
{
    /* copy list */
    for (uint32_t i = 0; i < in_component_count; i++) {
        components[i] = in_components[i];
    }
    *component_count = in_component_count;
    /* TODO: remove duplicated components */
}

nu_result_t nuecs_scene_initialize(nuecs_scene_data_t *scene)
{
    /* allocate resources */
    nu_array_allocate(&scene->entries, sizeof(nuecs_entity_entry_t));
    nu_array_allocate(&scene->free_entries, sizeof(uint32_t));
    nu_array_allocate(&scene->deleted_entries, sizeof(uint32_t));
    nu_array_allocate(&scene->chunks_to_update, sizeof(nuecs_chunk_data_t*));
    nuecs_archetype_table_initialize(&scene->archetype_table);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_terminate(nuecs_scene_data_t *scene)
{
    /* entities */
    nu_array_free(scene->entries);
    nu_array_free(scene->free_entries);
    nu_array_free(scene->deleted_entries);
    nu_array_free(scene->chunks_to_update);
    nuecs_archetype_table_terminate(scene->archetype_table);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_progress(nuecs_scene_t handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)handle;

    /* remove entities */
    uint32_t *indices    = (uint32_t*)nu_array_get_data_const(scene->deleted_entries);
    uint32_t index_count = nu_array_get_size(scene->deleted_entries);
    nuecs_entity_entry_t *entries = (nuecs_entity_entry_t*)nu_array_get_data(scene->entries);
    for (uint32_t i = 0; i < index_count; i++) {
        nuecs_chunk_remove(entries->chunk, entries->id);
        nu_array_push(scene->free_entries, &indices[i]);
    }
    nu_array_clear(scene->deleted_entries);

    // /* update chunk frame size */
    // nuecs_archetype_data_t **archetypes = (nuecs_archetype_data_t**)nu_array_get_data(scene->archetypes);
    // uint32_t archetype_count            = nu_array_get_size(scene->archetypes);
    // for (uint32_t i = 0; i < archetype_count; i++) {
    //     nuecs_chunk_data_t **chunks = (nuecs_chunk_data_t**)nu_array_get_data(archetypes[i]->chunks);
    //     uint32_t chunk_count        = nu_array_get_size(archetypes[i]->chunks);
    //     for (uint32_t j = 0; j < chunk_count; j++) {
    //         chunks[j]->frame_size = chunks[j]->size;
    //     }
    // }

    // /* update all systems */
    // nuecs_system_data_t **systems = (nuecs_system_data_t**)nu_indexed_array_get_data(scene->systems);
    // uint32_t system_count         = nu_indexed_array_get_size(scene->systems);
    // for (uint32_t i = 0; i < system_count; i++) {
    //     nuecs_system_update(systems[i]);
    // }

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_register_system(nuecs_scene_t scene_handle, const nuecs_system_info_t* info, nuecs_system_t* handle)
{
    // nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    // /* sort types ids */
    // nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
    // uint32_t component_count;
    // sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

    // /* allocate memory */
    // nuecs_system_data_t *system = (nuecs_system_data_t*)nu_malloc(sizeof(nuecs_system_data_t));
    // nuecs_system_create(system, components, component_count);
    // system->update = info->update;

    // /* add system to archetypes */
    // nuecs_archetype_data_t **archetypes = (nuecs_archetype_data_t**)nu_array_get_data(scene->archetypes);
    // uint32_t archetype_count            = nu_array_get_size(scene->archetypes);
    // for (uint32_t i = 0; i < archetype_count; i++) {
    //     nuecs_archetype_new_system(archetypes[i], system);
    // }

    /* save the system */
    // nu_indexed_array_add(scene->systems, &system, &system->id);

    /* set handle */
    // *handle = (nuecs_system_t)system;

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_create_entity(nuecs_scene_t scene_handle, const nuecs_entity_info_t* info, nuecs_entity_t* handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    /* sanatize components */
    nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;
    sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

    /* find the entry */
    uint32_t index;
    if (!nu_array_is_empty(scene->free_entries)) {
        index = *(uint32_t*)nu_array_get_last(scene->free_entries);
        nu_array_pop(scene->free_entries);
    } else {
        nu_array_push(scene->entries, NULL);
        index = nu_array_get_size(scene->entries) - 1;
    }
    nuecs_entity_entry_t *entry = (nuecs_entity_entry_t*)nu_array_get(scene->entries, index);
    NU_HANDLE_SET_ID(*handle, index);

    /* find archetype */
    nuecs_archetype_data_t *archetype;
    nuecs_component_manager_find_archetype(components, component_count, &archetype);

    /* find chunk */
    nuecs_chunk_data_t *chunk;
    nuecs_archetype_table_get_chunk(scene->archetype_table, archetype, &chunk);

    /* add to chunk */
    uint32_t id;
    nuecs_chunk_add(chunk, &id);
    entry->chunk = chunk;
    entry->id    = id;

    /* copy components */
    for (uint32_t i = 0; i < component_count; i++) {
        /* find component index */
        uint32_t component_index;
        nuecs_archetype_find_component_index(archetype, components[i]->id, &component_index);
        /* write component */
        nuecs_chunk_write_component(chunk, id, component_index, info->component_data[i]);
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_destroy_entity(nuecs_scene_t scene_handle, nuecs_entity_t handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    uint32_t index;
    NU_HANDLE_GET_ID(handle, index);
    nu_array_push(scene->deleted_entries, &index);

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
nu_result_t nuecs_scene_entity_add_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t data)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    /* get entity entry */
    uint32_t index;
    NU_HANDLE_GET_ID(handle, index);
    nuecs_entity_entry_t *entry = (nuecs_entity_entry_t*)nu_array_get(scene->entries, index);

    /* get component data */
    nuecs_component_data_t *component_data = (nuecs_component_data_t*)component;

    /* check entity has component */
    if (archetype_has_component(entry->chunk->archetype, component_data)) return NU_FAILURE;

    /* find next archetype */
    nuecs_archetype_data_t *next_archetype;
    nuecs_archetype_find_next(entry->chunk->archetype, component_data, &next_archetype);

    /* find next chunk */
    nuecs_chunk_data_t *next_chunk;
    nuecs_archetype_table_get_chunk(scene->archetype_table, next_archetype, &next_chunk);

    /* transfer data */
    uint32_t new_id;
    nuecs_chunk_transfer(entry->chunk, next_chunk, entry->id, &new_id);

    /* write new component */
    uint32_t component_index;
    nuecs_archetype_find_component_index(next_archetype, component_data->id, &component_index);
    nuecs_chunk_write_component(next_chunk, new_id, component_index, data);

    /* set new entry */
    entry->chunk = next_chunk;
    entry->id    = new_id;

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_entity_remove_component(nuecs_scene_t scene_handle, nuecs_entity_t handle, nuecs_component_t component)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    /* get entity entry */
    uint32_t index;
    NU_HANDLE_GET_ID(handle, index);
    nuecs_entity_entry_t *entry = (nuecs_entity_entry_t*)nu_array_get(scene->entries, index);

    /* get component data */
    nuecs_component_data_t *component_data = (nuecs_component_data_t*)component;

    /* check entity has component */
    if (!archetype_has_component(entry->chunk->archetype, component_data)) return NU_FAILURE;

    /* find previous archetype */
    nuecs_archetype_data_t *previous_archetype;
    nuecs_archetype_find_previous(entry->chunk->archetype, component_data, &previous_archetype);    

    /* find previous chunk */
    nuecs_chunk_data_t *previous_chunk;
    nuecs_archetype_table_get_chunk(scene->archetype_table, previous_archetype, &previous_chunk);

    /* transfer data */
    uint32_t new_id;
    nuecs_chunk_transfer(entry->chunk, previous_chunk, entry->id, &new_id);

    /* set new entry */
    entry->chunk = previous_chunk;
    entry->id    = new_id;

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_save_file(nuecs_scene_t scene_handle, const char* filename)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    /* allocate json */
    nu_json_t json;
    nu_json_allocate_empty_object(&json);
    nu_json_object_t root;
    nu_json_value_as_object(nu_json_get_root(json), &root);

    /* put entities array */
    nu_json_array_t j_entities;
    nu_json_object_put_empty_array(root, "entities", &j_entities);

    /* iterate over entries */
    uint32_t entry_count                = nu_array_get_size(scene->entries);
    const nuecs_entity_entry_t *entries = (const nuecs_entity_entry_t*)nu_array_get_data_const(scene->entries);
    for (uint32_t i = 0; i < entry_count; i++) {

        /* create entity object */
        nu_json_object_t j_entity;
        nu_json_array_add_empty_object(j_entities, &j_entity);

        /* put id */
        nu_json_object_put_uint(j_entity, "id", i);

        /* put components */
        nu_json_object_t j_components;
        nu_json_object_put_empty_object(j_entity, "components", &j_components);

        /* iterate over components */
        const nuecs_archetype_data_t *archetype = entries[i].chunk->archetype;
        for (uint32_t j = 0; j < archetype->component_count; j++) {
            uint32_t id = archetype->component_ids[j];

            /* get component */
            nuecs_component_data_t *component;
            nuecs_component_manager_get_component(id, &component);

            /* put component data */
            nu_json_object_t j_component;
            nu_json_object_put_empty_object(j_components, nu_string_get_cstr(component->name), &j_component);

            if (component->serialize_json) {
                /* find component data */
                uint32_t index;
                nuecs_archetype_find_component_index(archetype, id, &index);
                nuecs_component_data_ptr_t data;
                nuecs_chunk_get_component(entries[i].chunk, entries[i].id, index, &data);

                /* serialize component */
                component->serialize_json(data, j_component);   
            }
        }
    }

    /* save json */
    nu_result_t result = nu_json_save_file(json, filename, false);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUECS_LOGGER_NAME, "Failed to save scene.");

cleanup0:
    nu_json_free(json);

    return result;
}