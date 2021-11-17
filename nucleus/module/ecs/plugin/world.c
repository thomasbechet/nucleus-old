#include <nucleus/module/ecs/plugin/world.h>

#include <nucleus/module/ecs/plugin/archetype.h>

static int nuecs_component_type_cmp(const void* a, const void* b)
{
    nuecs_component_type_t *ta = *((nuecs_component_type_t**)a);
    nuecs_component_type_t *tb = *((nuecs_component_type_t**)b);

    if (ta->id == tb->id) return 0;
    else if (ta->id < tb->id) return -1;
    else return 1;
}

static void nuecs_sanatize_components(const nuecs_entity_info_t *info, nuecs_component_type_t **types, uint32_t *type_count)
{
    for (uint32_t i = 0; i < info->component_count; i++) {
        types[i] = (nuecs_component_type_t*)info->components[i];
    }
    qsort(types, info->component_count, sizeof(nuecs_component_type_t*), nuecs_component_type_cmp);
    *type_count = info->component_count;
}

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world)
{
    nu_array_allocate(&world->archetypes, sizeof(nuecs_archetype_t*));
    nu_array_allocate(&world->types, sizeof(nuecs_component_type_t*));
    nu_indexed_array_allocate(&world->entities, sizeof(nuecs_entity_data_t));

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
    
    /* types */
    nuecs_component_type_t **types = (nuecs_component_type_t**)nu_array_get_data(world->types);
    uint32_t type_count = nu_array_get_size(world->types);
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_free(types[i]->name);
        nu_free(types[i]);
    }
    nu_array_free(world->types);

    /* entities */
    nu_indexed_array_free(world->entities);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_component_register(nuecs_world_data_t *world, const nuecs_component_info_t *info, nuecs_component_t *handle)
{    
    nuecs_component_type_t *type = (nuecs_component_type_t*)nu_malloc(sizeof(nuecs_component_type_t));
    type->id = world->next_type_id++;
    nu_string_allocate_cstr(&type->name, info->name);
    type->size = info->size;

    nu_array_push(world->types, &type);

    *handle = (nuecs_component_t)type;

    return NU_SUCCESS;
}
nu_result_t nuecs_world_system_register(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_create(nuecs_world_data_t *world, const nuecs_entity_info_t *info, nuecs_entity_t *handle)
{
    nuecs_component_type_t *types[NUECS_MAX_COMPONENT_PER_ENTITY];
    uint32_t type_count;
    nuecs_sanatize_components(info, types, &type_count);

    /* allocate entity data */
    uint32_t entity_id;
    nu_indexed_array_add(world->entities, NULL, &entity_id);
    nuecs_entity_data_t *entity = (nuecs_entity_data_t*)nu_indexed_array_get(world->entities, entity_id);
    NU_HANDLE_SET_ID(*handle, entity_id);

    /* add/find archetype */
    nuecs_archetype_t *archetype;
    bool created;
    nuecs_archetype_find(world->archetypes, world->empty_archetype, types, type_count, &archetype, &created);
    
    /* reconfigure systems */
    if (created) {

    }

    /* add entity to archetype */
    nuecs_archetype_add(archetype, info->data, entity);

    nu_info("NEW ENTITY", "archetype %p chunk %d id %d", entity->archetype, entity->chunk, entity->row);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_destroy(nuecs_world_data_t *world, nuecs_entity_t handle)
{
    return NU_SUCCESS;
}