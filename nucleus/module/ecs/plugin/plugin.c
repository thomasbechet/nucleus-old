#include <nucleus/module/ecs/plugin/plugin.h>

#include <nucleus/module/ecs/plugin/world.h>

typedef struct {
    nu_indexed_array_t worlds;
} nuecs_module_data_t;

static nuecs_module_data_t _module;

nu_result_t nuecs_world_plugin_initialize(void)
{
    nu_indexed_array_allocate(&_module.worlds, sizeof(nuecs_world_data_t*));

    return NU_SUCCESS;
}
nu_result_t nuecs_world_plugin_terminate(void)
{
    nuecs_world_data_t **worlds = (nuecs_world_data_t**)nu_indexed_array_get_data(_module.worlds);
    uint32_t world_count = nu_indexed_array_get_size(_module.worlds);
    for (uint32_t i = 0; i < world_count; i++) {
        nuecs_world_terminate(worlds[i]);
        nu_free(worlds[i]);
    }
    nu_indexed_array_free(_module.worlds);

    return NU_SUCCESS;
}
nu_result_t nuecs_world_plugin_update(void)
{
    nuecs_world_data_t **worlds = (nuecs_world_data_t**)nu_indexed_array_get_data(_module.worlds);
    uint32_t world_count        = nu_indexed_array_get_size(_module.worlds);
    for (uint32_t i = 0; i < world_count; i++) {
        nuecs_world_progress((nuecs_world_t)worlds[i]);
    }
    return NU_SUCCESS;
}

nu_result_t nuecs_world_create(nuecs_world_t* handle)
{
    nuecs_world_data_t *world = (nuecs_world_data_t*)nu_malloc(sizeof(nuecs_world_data_t));
    nu_indexed_array_add(_module.worlds, &world, &world->id);
    *handle = (nuecs_world_t)world;
    return nuecs_world_initialize(world);
}
nu_result_t nuecs_world_destroy(nuecs_world_t handle)
{
    return NU_SUCCESS;
}