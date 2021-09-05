#include <nucleus/module/vulkan/sdf/pipeline/pipelines.h>

#include <nucleus/module/vulkan/sdf/pipeline/geometry.h>
#include <nucleus/module/vulkan/sdf/pipeline/postprocess.h>

nu_result_t nuvk_sdf_pipelines_initialize(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_pipeline_sources_load(pipelines->sources);

    result &= nuvk_sdf_pipeline_geometry_create(&pipelines->geometry, context,
        shader_manager, descriptors, renderpasses->geometry, pipelines->sources);
    result &= nuvk_sdf_pipeline_postprocess_create(&pipelines->postprocess, context,
        shader_manager, descriptors, renderpasses->postprocess, pipelines->sources);

    return result;
}
nu_result_t nuvk_sdf_pipelines_terminate(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context
)
{
    nuvk_sdf_pipeline_geometry_destroy(&pipelines->geometry, context);
    nuvk_sdf_pipeline_postprocess_destroy(&pipelines->postprocess, context);

    nuvk_sdf_pipeline_sources_unload(pipelines->sources);

    return NU_SUCCESS;
}