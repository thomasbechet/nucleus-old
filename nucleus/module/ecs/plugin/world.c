#include <nucleus/module/ecs/plugin/world.h>

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world)
{
    nu_array_allocate(&world->archetypes, sizeof(nuecs_archetype_t*));
    nu_indexed_array_allocate(&world->types, sizeof(nuecs_component_data_t));

    return NU_SUCCESS;
}
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world)
{
    nu_array_free(world->archetypes);
    nu_indexed_array_free(world->types);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_register_component(nuecs_world_data_t *world, const nuecs_component_info_t *info, nuecs_component_t *handle)
{
    uint32_t id;
    nu_indexed_array_add(world->types, NULL, &id);
    nuecs_component_data_t *data = (nuecs_component_data_t*)nu_indexed_array_get(world->types, id);
    

    return NU_SUCCESS;
}
nu_result_t nuecs_world_register_system(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_create(nuecs_world_data_t *world, const nuecs_entity_info_t *info, nuecs_entity_t *handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_world_entity_destroy(nuecs_world_data_t *world, nuecs_entity_t handle)
{
    return NU_SUCCESS;
}