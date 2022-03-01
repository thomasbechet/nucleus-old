#include <nucleus/module/ecs/plugin/plugin.h>

#include <nucleus/module/ecs/plugin/scene_manager.h>
#include <nucleus/module/ecs/plugin/component_manager.h>

/* plugin */
nu_result_t nuecs_scene_plugin_initialize(void)
{
    nuecs_component_manager_initialize();
    nuecs_scene_manager_initialize();
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_terminate(void)
{
    nuecs_scene_manager_terminate();
    nuecs_component_manager_terminate();
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_plugin_update(void)
{
    return nuecs_scene_manager_progress();
    return NU_SUCCESS;
}