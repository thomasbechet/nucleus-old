#include <nucleus/module/ecs/plugin/scene_manager.h>

#include <nucleus/module/ecs/plugin/scene.h>

static nuecs_scene_manager_data_t _manager;

nu_result_t nuecs_scene_manager_initialize(void)
{
    nu_indexed_array_allocate(&_manager.scenes, sizeof(nuecs_scene_data_t*));

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_manager_terminate(void)
{
    nuecs_scene_data_t **scenes = (nuecs_scene_data_t**)nu_indexed_array_get_data(_manager.scenes);
    uint32_t scene_count = nu_indexed_array_get_size(_manager.scenes);
    for (uint32_t i = 0; i < scene_count; i++) {
        nuecs_scene_terminate(scenes[i]);
        nu_free(scenes[i]);
    }
    nu_indexed_array_free(_manager.scenes);

    return NU_SUCCESS;
}
nu_result_t nuecs_scene_manager_create_scene(nuecs_scene_t* handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)nu_malloc(sizeof(nuecs_scene_data_t));
    nu_indexed_array_add(_manager.scenes, &scene, &scene->id);
    *handle = (nuecs_scene_t)scene;
    return nuecs_scene_initialize(scene);
}
nu_result_t nuecs_scene_manager_destroy_scene(nuecs_scene_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_scene_manager_progress(void)
{
    nuecs_scene_data_t **scenes = (nuecs_scene_data_t**)nu_indexed_array_get_data(_manager.scenes);
    uint32_t scene_count        = nu_indexed_array_get_size(_manager.scenes);
    for (uint32_t i = 0; i < scene_count; i++) {
        nuecs_scene_progress((nuecs_scene_t)scenes[i]);
    }
    return NU_SUCCESS;
}