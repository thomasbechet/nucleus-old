#include <nucleus/module/ecs/plugin/world.h>

#include <nucleus/module/ecs/plugin/archetype.h>

static nuecs_world_find_archetype(nuecs_id_t *types, uint32_t type_count)
{

}

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world)
{
    nu_array_allocate(&world->archetypes, sizeof(nuecs_archetype_t*));
    nu_indexed_array_allocate(&world->types, sizeof(nuecs_component_type_t));

    /* create empty archetype */
    nuecs_archetype_create(&world->empty_archetype, NULL, 0);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world)
{
    /* destoy empty archetype */
    nuecs_archetype_destroy(&world->empty_archetype);

    /* free resources */
    nu_array_free(world->archetypes);
    nuecs_component_type_t *types = (nuecs_component_type_t*)nu_indexed_array_get_data(world->types);
    uint32_t type_count = nu_indexed_array_get_size(world->types);
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_free(types[i].name);
    }
    nu_indexed_array_free(world->types);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_register_component(nuecs_world_data_t *world, const nuecs_component_info_t *info, nuecs_component_t *handle)
{
    uint32_t id;
    nu_indexed_array_add(world->types, NULL, &id);
    nuecs_component_type_t *type = (nuecs_component_type_t*)nu_indexed_array_get(world->types, id);
    
    nu_string_allocate_cstr(&type->name, info->name);
    type->size = info->size;

    NU_HANDLE_SET_ID(*handle, id);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_register_system(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_create(nuecs_world_data_t *world, const nuecs_entity_info_t *info, nuecs_entity_t *handle)
{
    nuecs_id_t *types = (nuecs_id_t*)nu_malloc(sizeof(nuecs_id_t) * info->component_count);
    for (uint32_t i = 0; i < info->component_count; i++) {
        types[i] = (nuecs_id_t)info->components[i];
    }

    

    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_destroy(nuecs_world_data_t *world, nuecs_entity_t handle)
{
    return NU_SUCCESS;
}