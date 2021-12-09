#include <nucleus/module/ecs/plugin/world.h>

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/system.h>
#include <nucleus/module/ecs/plugin/chunk.h>

static int nuecs_component_type_cmp(const void* a, const void* b)
{
    nuecs_component_type_t *ta = *((nuecs_component_type_t**)a);
    nuecs_component_type_t *tb = *((nuecs_component_type_t**)b);

    if (ta->type_id == tb->type_id) return 0;
    else if (ta->type_id < tb->type_id) return -1;
    else return 1;
}
static void nuecs_sanatize_components(
    nuecs_component_type_t **in_types,
    uint32_t in_type_count,
    nuecs_component_type_t **types,
    uint32_t *type_count
)
{
    for (uint32_t i = 0; i < in_type_count; i++) {
        types[i] = in_types[i];
    }
    qsort(types, in_type_count, sizeof(nuecs_component_type_t*), nuecs_component_type_cmp);
    *type_count = in_type_count;
    /* TODO: remove duplicated components */
}

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world)
{
    /* allocate resources */
    nu_array_allocate(&world->archetypes, sizeof(nuecs_archetype_t*));
    nu_array_allocate(&world->entries, sizeof(nuecs_entity_entry_t));
    nu_array_allocate(&world->free_entries, sizeof(uint32_t));
    nu_array_allocate(&world->deleted_entries, sizeof(uint32_t));
    nu_indexed_array_allocate(&world->types, sizeof(nuecs_component_type_t*));
    nu_indexed_array_allocate(&world->systems, sizeof(nuecs_system_data_t*));

    /* first type id is 0 */
    world->next_type_id = 0;

    /* create empty archetype */
    nuecs_archetype_t *empty_archetype = (nuecs_archetype_t*)nu_malloc(sizeof(nuecs_archetype_t));
    nuecs_archetype_create(empty_archetype, NULL, 0);
    nu_array_push(world->archetypes, &empty_archetype);
    world->empty_archetype = empty_archetype;

    return NU_SUCCESS;
}
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world)
{
    /* archetypes */
    nuecs_archetype_t **archetypes = (nuecs_archetype_t**)nu_array_get_data(world->archetypes);
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

    /* types */
    nuecs_component_type_t **types = (nuecs_component_type_t**)nu_indexed_array_get_data(world->types);
    uint32_t type_count = nu_indexed_array_get_size(world->types);
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_free(types[i]->name);
        nu_free(types[i]);
    }
    nu_indexed_array_free(world->types);

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
    nuecs_component_type_t *type = (nuecs_component_type_t*)nu_malloc(sizeof(nuecs_component_type_t));
    type->type_id = world->next_type_id++;
    nu_string_allocate_cstr(&type->name, info->name);
    type->size = info->size;

    /* save the type */
    nu_indexed_array_add(world->types, &type, &type->id);

    /* set the handle */
    *handle = (nuecs_component_t)type;

    return NU_SUCCESS;
}
nu_result_t nuecs_world_system_register(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle)
{
    /* sort types ids */
    nuecs_component_type_t *types[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t type_count;
    nuecs_sanatize_components((nuecs_component_type_t**)info->components, info->component_count, types, &type_count);

    /* allocate memory */
    nuecs_system_data_t *system = (nuecs_system_data_t*)nu_malloc(sizeof(nuecs_system_data_t));
    nuecs_system_create(system, types, type_count);
    system->update = info->update;

    /* add system to archetypes */
    nuecs_archetype_t **archetypes = (nuecs_archetype_t**)nu_array_get_data(world->archetypes);
    uint32_t archetype_count       = nu_array_get_size(world->archetypes);
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
    nuecs_component_type_t *types[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t type_count;
    nuecs_sanatize_components((nuecs_component_type_t**)info->components, info->component_count, types, &type_count);

    /* allocate entity data */
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

    /* add/find archetype */
    nuecs_archetype_t *archetype;
    nuecs_archetype_find(world->archetypes, world->empty_archetype, types, type_count, 
        (nuecs_system_data_t**)nu_indexed_array_get_data(world->systems), nu_indexed_array_get_size(world->systems),
        &archetype);

    /* add entity to archetype */
    nuecs_archetype_add_entity(archetype, info->data, entry);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_destroy(nuecs_world_data_t *world, nuecs_entity_t handle)
{
    uint32_t index;
    NU_HANDLE_GET_ID(handle, index);
    nu_array_push(world->deleted_entries, &index);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_update(nuecs_world_data_t *world)
{
    /* remove entities */
    uint32_t *indices    = (uint32_t*)nu_array_get_data_const(world->deleted_entries);
    uint32_t index_count = nu_array_get_size(world->deleted_entries);
    nuecs_entity_entry_t *entries = (nuecs_entity_entry_t*)nu_array_get_data(world->entries);
    for (uint32_t i = 0; i < index_count; i++) {
        nuecs_chunk_remove(entries[i].chunk, entries[i].chunk_id);
        nu_array_push(world->free_entries, &indices[i]);
    }
    nu_array_clear(world->deleted_entries);

    /* update chunk frame size */
    nuecs_archetype_t **archetypes = (nuecs_archetype_t**)nu_array_get_data(world->archetypes);
    uint32_t archetype_count       = nu_array_get_size(world->archetypes);
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