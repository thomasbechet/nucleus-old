#include <nucleus/module/ecs/plugin/world.h>

nu_result_t nuecs_world_initialize(nuecs_world_data_t *world)
{
    nu_array_allocate(&world->archetypes, sizeof(nuecs_archetype_data_t*));

    return NU_SUCCESS;
}
nu_result_t nuecs_world_terminate(nuecs_world_data_t *world)
{
    nu_array_free(world->archetypes);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_register_component(nuecs_world_data_t *world, const nuecs_component_info_t *info, nuecs_component_t *handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_world_register_system(nuecs_world_data_t *world, const nuecs_system_info_t *info, nuecs_system_t *handle)
{
    return NU_SUCCESS;
}