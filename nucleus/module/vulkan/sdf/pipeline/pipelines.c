#include <nucleus/module/vulkan/sdf/pipeline/pipelines.h>

nu_result_t nuvk_sdf_pipelines_initialize(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shaders_t *shaders,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_pipeline_postprocess_create(&pipelines->postprocess, context, swapchain,
        &shaders->postprocess,  descriptors, renderpasses->postprocess);

    return result;
}
nu_result_t nuvk_sdf_pipelines_terminate(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context
)
{
    nuvk_sdf_pipeline_postprocess_destroy(&pipelines->postprocess, context);

    return NU_SUCCESS;
}