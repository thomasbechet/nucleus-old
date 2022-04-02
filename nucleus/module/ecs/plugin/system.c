#include <nucleus/module/ecs/plugin/system.h>

nu_result_t nuecs_system_initialize(
    nuecs_system_data_t *system,
    const nuecs_system_info_t *info
)
{
    nu_string_allocate_cstr(&system->name, info->name);
    return NU_SUCCESS;
}
nu_result_t nuecs_system_terminate(
    nuecs_system_data_t *system
)
{
    nu_string_free(system->name);
    return NU_SUCCESS;
}

nu_result_t nuecs_pipeline_initialize(
    nuecs_pipeline_data_t *pipeline,
    const nuecs_pipeline_info_t *info
)
{
    

    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_terminate(
    nuecs_pipeline_data_t *pipeline
)
{


    return NU_SUCCESS;
}