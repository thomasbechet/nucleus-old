#include <nucleus/module/ecs/plugin/system_manager.h>

#include <nucleus/module/ecs/plugin/system.h>
#include <nucleus/module/ecs/plugin/pipeline.h>

nu_result_t nuecs_system_manager_initialize(nuecs_system_manager_data_t *manager)
{
    nu_array_allocate(&manager->systems, sizeof(nuecs_system_data_t));
    nu_array_allocate(&manager->pipelines, sizeof(nuecs_pipeline_data_t));

    return NU_SUCCESS;
}
nu_result_t nuecs_system_manager_terminate(nuecs_system_manager_data_t *manager)
{
    /* systems */
    nuecs_system_data_t *systems; uint32_t system_count;
    nu_array_get_data(manager->systems, &systems, &system_count);
    for (uint32_t i = 0; i < system_count; i++) {
        nuecs_system_terminate(&systems[i]);
    }
    nu_array_free(manager->systems);

    /* pipelines */
    nuecs_pipeline_data_t *pipelines; uint32_t pipeline_count;
    nu_array_get_data(manager->pipelines, &pipelines, &pipeline_count);
    for (uint32_t i = 0; i < pipeline_count; i++) {
        nuecs_pipeline_terminate(&pipelines[i]);
    }
    nu_array_free(manager->pipelines);

    return NU_SUCCESS;
}

nu_result_t nuecs_system_manager_build_system(
    nuecs_system_manager_data_t *manager,
    const nuecs_system_info_t *info, 
    nuecs_system_t *handle
)
{
    nuecs_system_data_t *system;
    nu_array_push(manager->systems, NULL);
    nu_array_get_last(manager->systems, &system);
    NU_HANDLE_SET_ID(*handle, nu_array_get_size(manager->systems) - 1);
    return nuecs_system_initialize(system, info);
}
nu_result_t nuecs_system_manager_build_pipeline(
    nuecs_system_manager_data_t *manager,
    const nuecs_pipeline_info_t *info, 
    nuecs_pipeline_t *handle
)
{
    nuecs_pipeline_data_t *pipeline;
    nu_array_push(manager->pipelines, NULL);
    nu_array_get_last(manager->pipelines, &pipeline);
    NU_HANDLE_SET_ID(*handle, nu_array_get_size(manager->pipelines) - 1);
    return nuecs_pipeline_initialize(pipeline, info);
}