#include <nucleus/module/vulkan/sdf/pipeline/pipelines.h>

#include <nucleus/module/vulkan/sdf/pipeline/geometry.h>
#include <nucleus/module/vulkan/sdf/pipeline/postprocess.h>

nu_result_t nuvk_sdf_pipelines_initialize(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shaders_t *shaders,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_pipeline_geometry_create(&pipelines->geometry, context, swapchain,
        &shaders->geometry, renderpasses->geometry);
    result &= nuvk_sdf_pipeline_postprocess_create(&pipelines->postprocess, context, swapchain,
        &shaders->postprocess, renderpasses->postprocess);

    return result;
}
nu_result_t nuvk_sdf_pipelines_terminate(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context
)
{
    vkDestroyPipeline(context->device, pipelines->postprocess, &context->allocator);
    vkDestroyPipeline(context->device, pipelines->geometry, &context->allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_pipelines_update_swapchain(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shaders_t *shaders,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    vkDestroyPipeline(context->device, pipelines->postprocess, &context->allocator);
    vkDestroyPipeline(context->device, pipelines->geometry, &context->allocator);

    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_pipeline_geometry_create(&pipelines->geometry, context, swapchain,
        &shaders->geometry, renderpasses->geometry);
    result &= nuvk_sdf_pipeline_postprocess_create(&pipelines->postprocess, context, swapchain,
        &shaders->postprocess, renderpasses->postprocess);

    return result;
}