#include <nucleus/module/ecs/plugin/world.h>

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/system.h>
#include <nucleus/module/ecs/plugin/chunk.h>

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

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world)
{
    /* allocate resources */
    nu_array_allocate(&world->archetypes, sizeof(nuecs_archetype_t*));
    nu_array_allocate(&world->entries, sizeof(nuecs_entity_entry_t));
    nu_array_allocate(&world->free_entries, sizeof(uint32_t));
    nu_array_allocate(&world->deleted_entries, sizeof(uint32_t));
    nu_indexed_array_allocate(&world->components, sizeof(nuecs_component_data_t*));
    nu_indexed_array_allocate(&world->systems, sizeof(nuecs_system_data_t*));

    /* first component id is 0 */
    world->next_component_id = 0;

    /* create empty archetype */
    nuecs_archetype_data_t *empty_archetype = (nuecs_archetype_data_t*)nu_malloc(sizeof(nuecs_archetype_data_t));
    nuecs_archetype_create_empty(empty_archetype);
    nu_array_push(world->archetypes, &empty_archetype);
    world->empty_archetype = empty_archetype;

    return NU_SUCCESS;
}
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world)
{
    /* archetypes */
    nuecs_archetype_data_t **archetypes = (nuecs_archetype_data_t**)nu_array_get_data(world->archetypes);
    uint32_t archetype_count = nu_array_get_size(world->archetypes);
    for (uint32_t i = 0; i < archetype_count; i++) {
        nuecs_archetype_destroy(archetypes[i]);
        nu_free(archetypes[i]);
    }
    nu_array_free(world->archetypes);
    
    /* entities */
    nu_array_free(world->entries);
    nu_array_free(world->free_entries);
    nu_array_free(world->deleted_entries);

    /* components */
    nuecs_component_data_t **components = (nuecs_component_data_t**)nu_indexed_array_get_data(world->components);
    uint32_t type_count = nu_indexed_array_get_size(world->components);
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_free(components[i]->name);
        nu_free(components[i]);
    }
    nu_indexed_array_free(world->components);

    /* systems */
    nuecs_system_data_t **systems = (nuecs_system_data_t**)nu_indexed_array_get_data(world->systems);
    uint32_t system_count = nu_indexed_array_get_size(world->systems);
    for (uint32_t i = 0; i < system_count; i++) {
        nuecs_system_destroy(systems[i]);
        nu_free(systems[i]);
    }
    nu_indexed_array_free(world->systems);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_component_register(nuecs_world_data_t *world, const nuecs_component_info_t *info, nuecs_component_t *handle)
{    
    /* allocate memory */
    nuecs_component_data_t *component = (nuecs_component_data_t*)nu_malloc(sizeof(nuecs_component_data_t));
    component->component_id = world->next_component_id++;
    nu_string_allocate_cstr(&component->name, info->name);
    component->size = info->size;

    /* save the component */
    nu_indexed_array_add(world->components, &component, &component->id);

    /* set the handle */
    *handle = (nuecs_component_t)component;

    return NU_SUCCESS;
}
nu_result_t nuecs_world_system_register(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle)
{
    /* sort types ids */
    nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;
    sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

    /* allocate memory */
    nuecs_system_data_t *system = (nuecs_system_data_t*)nu_malloc(sizeof(nuecs_system_data_t));
    nuecs_system_create(system, components, component_count);
    system->update = info->update;

    /* add system to archetypes */
    nuecs_archetype_data_t **archetypes = (nuecs_archetype_data_t**)nu_array_get_data(world->archetypes);
    uint32_t archetype_count            = nu_array_get_size(world->archetypes);
    for (uint32_t i = 0; i < archetype_count; i++) {
        nuecs_archetype_new_system(archetypes[i], system);
    }

    /* save the system */
    nu_indexed_array_add(world->systems, &system, &system->id);

    /* set handle */
    *handle = (nuecs_system_t)system;

    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_create(nuecs_world_data_t *world, const nuecs_entity_info_t *info, nuecs_entity_t *handle)
{
    nuecs_component_data_t *components[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t component_count;
    sanatize_components((nuecs_component_data_t**)info->components, info->component_count, components, &component_count);

    /* find the entry */
    uint32_t index;
    if (!nu_array_is_empty(world->free_entries)) {
        index = *(uint32_t*)nu_array_get_last(world->free_entries);
        nu_array_pop(world->free_entries);
    } else {
        nu_array_push(world->entries, NULL);
        index = nu_array_get_size(world->entries) - 1;
    }
    nuecs_entity_entry_t *entry = (nuecs_entity_entry_t*)nu_array_get(world->entries, index);
    NU_HANDLE_SET_ID(*handle, index);

    /* find the archetype */
    nuecs_archetype_data_t *archetype;
    nuecs_archetype_find(world->archetypes, world->empty_archetype, components, component_count, 
        (nuecs_system_data_t**)nu_indexed_array_get_data(world->systems), nu_indexed_array_get_size(world->systems),
        &archetype);

    /* add entity to archetype */
    nuecs_archetype_add_entity(archetype, (const nuecs_component_data_t**)components, info->component_data, component_count, entry);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_destroy(nuecs_world_data_t *world, nuecs_entity_t handle)
{
    uint32_t index;
    NU_HANDLE_GET_ID(handle, index);
    nu_array_push(world->deleted_entries, &index);

    return NU_SUCCESS;
}
static bool archetype_has_component(const nuecs_archetype_data_t *archetype, const nuecs_component_data_t *component)
{
    for (uint32_t i = 0; i < archetype->component_count; i++) {
        if (component->component_id == archetype->component_ids[i]) {
            return true;
        }
    }
    return false;
}
nu_result_t nuecs_world_entity_add_component(nuecs_world_data_t *world, nuecs_entity_t handle, nuecs_component_t component, nuecs_component_data_ptr_t data)
{
    /* get entity entry */
    uint32_t index;
    NU_HANDLE_GET_ID(handle, index);
    nuecs_entity_entry_t *entry = (nuecs_entity_entry_t*)nu_array_get(world->entries, index);

    /* get component data */
    nuecs_component_data_t *component_data = (nuecs_component_data_t*)component;

    /* check entity has component */
    if (archetype_has_component(entry->chunk->archetype, component_data)) return NU_FAILURE;

    /* find new archetype */
    nuecs_system_data_t **systems = (nuecs_system_data_t**)nu_indexed_array_get_data(world->systems);
    uint32_t system_count         = nu_indexed_array_get_size(world->systems);
    nuecs_archetype_data_t *archetype;
    nuecs_archetype_find_next(world->archetypes, entry->chunk->archetype, 
        component_data, systems, system_count, &archetype);

    /* transfer entity */
    nuecs_archetype_transfer(entry, archetype);

    /* write new component */
    uint32_t component_index;
    nuecs_archetype_find_component_index(archetype, component_data->component_id, &component_index);
    nuecs_chunk_write_component(entry->chunk, entry->chunk_id, component_index, data);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_remove_component(nuecs_world_data_t *world, nuecs_entity_t handle, nuecs_component_t component)
{
    /* get entity entry */
    uint32_t index;
    NU_HANDLE_GET_ID(handle, index);
    nuecs_entity_entry_t *entry = (nuecs_entity_entry_t*)nu_array_get(world->entries, index);

    /* get component data */
    nuecs_component_data_t *component_data = (nuecs_component_data_t*)component;

    /* check entity has component */
    if (!archetype_has_component(entry->chunk->archetype, component_data)) return NU_FAILURE;

    /* find previous archetype */
    nuecs_archetype_data_t *archetype;
    nuecs_archetype_find_previous(world->archetypes, world->empty_archetype, entry->chunk->archetype, 
        component_data, &archetype);

    /* transfer entity */
    nuecs_archetype_transfer(entry, archetype);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_update(nuecs_world_data_t *world)
{
    /* remove entities */
    uint32_t *indices    = (uint32_t*)nu_array_get_data_const(world->deleted_entries);
    uint32_t index_count = nu_array_get_size(world->deleted_entries);
    nuecs_entity_entry_t *entries = (nuecs_entity_entry_t*)nu_array_get_data(world->entries);
    for (uint32_t i = 0; i < index_count; i++) {
        nuecs_archetype_remove_entity(&entries[i]);
        nu_array_push(world->free_entries, &indices[i]);
    }
    nu_array_clear(world->deleted_entries);

    /* update chunk frame size */
    nuecs_archetype_data_t **archetypes = (nuecs_archetype_data_t**)nu_array_get_data(world->archetypes);
    uint32_t archetype_count            = nu_array_get_size(world->archetypes);
    for (uint32_t i = 0; i < archetype_count; i++) {
        nuecs_chunk_data_t **chunks = (nuecs_chunk_data_t**)nu_array_get_data(archetypes[i]->chunks);
        uint32_t chunk_count        = nu_array_get_size(archetypes[i]->chunks);
        for (uint32_t j = 0; j < chunk_count; j++) {
            chunks[j]->frame_size = chunks[j]->size;
        }
    }

    /* update all systems */
    nuecs_system_data_t **systems = (nuecs_system_data_t**)nu_indexed_array_get_data(world->systems);
    uint32_t system_count         = nu_indexed_array_get_size(world->systems);
    for (uint32_t i = 0; i < system_count; i++) {
        nuecs_system_update(systems[i]);
    }

    return NU_SUCCESS;
}