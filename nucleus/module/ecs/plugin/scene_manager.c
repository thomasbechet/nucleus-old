#include <nucleus/module/ecs/plugin/scene_manager.h>

#include <nucleus/module/ecs/plugin/scene.h>

nu_result_t nuecs_scene_manager_initialize(nuecs_scene_manager_data_t *manager)
{
    nu_indexed_array_allocate(&manager->scenes, sizeof(nuecs_scene_data_t*));

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_manager_terminate(nuecs_scene_manager_data_t *manager)
{
    nuecs_scene_data_t **scenes;
    uint32_t scene_count;
    nu_indexed_array_get_data(manager->scenes, &scenes, &scene_count);
    for (uint32_t i = 0; i < scene_count; i++) {
        nuecs_scene_terminate(scenes[i]);
        nu_free(scenes[i]);
    }
    nu_indexed_array_free(manager->scenes);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_manager_progress(
    nuecs_scene_manager_data_t *scene_manager,
    nuecs_system_manager_data_t *system_manager,
    nuecs_component_manager_data_t *component_manager
)
{
    nuecs_scene_data_t **scenes; uint32_t scene_count;
    nu_indexed_array_get_data(scene_manager->scenes, &scenes, &scene_count);
    for (uint32_t i = 0; i < scene_count; i++) {
        nuecs_scene_progress(system_manager, component_manager, scenes[i]);
    }
    return NU_SUCCESS;
}