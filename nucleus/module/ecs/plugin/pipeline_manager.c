#include <nucleus/module/ecs/plugin/pipeline_manager.h>

#include <nucleus/module/ecs/plugin/pipeline.h>
#include <nucleus/module/ecs/plugin/component.h>

nu_result_t nuecs_pipeline_manager_initialize(nuecs_pipeline_manager_data_t *manager)
{
    nu_array_allocate(&manager->pipelines, sizeof(nuecs_pipeline_data_t));
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_manager_terminate(nuecs_pipeline_manager_data_t *manager)
{
    nuecs_pipeline_data_t *pipelines; uint32_t pipeline_count;
    nu_array_get_data(manager->pipelines, &pipelines, &pipeline_count);
    for (uint32_t i = 0; i < pipeline_count; i++) {
        nuecs_pipeline_terminate(&pipelines[i]);
    }
    nu_array_free(manager->pipelines);
    return NU_SUCCESS;
}

nu_result_t nuecs_pipeline_manager_build_component(
    nuecs_pipeline_manager_data_t *manager,
    const nuecs_pipeline_info_t *info, 
    nuecs_pipeline_t *handle
)
{
    /* allocate memory */
    nuecs_pipeline_data_t *pipeline;
    nu_array_push(manager->pipelines, NULL);
    nu_array_get_last(manager->pipelines, &pipeline);
    NU_HANDLE_SET_ID(*handle, nu_array_get_size(manager->pipelines) - 1);
    return nuecs_pipeline_initialize(pipeline, info);
}