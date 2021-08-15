#include <nucleus/module/vulkan/sdf/renderpass/renderpasses.h>

#include <nucleus/module/vulkan/sdf/renderpass/geometry.h>
#include <nucleus/module/vulkan/sdf/renderpass/postprocess.h>

nu_result_t nuvk_sdf_renderpasses_initialize(
    nuvk_sdf_renderpasses_t *renderpasses,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_images_t *images
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_renderpass_geometry_create(&renderpasses->geometry, context, &images->geometry);
    result &= nuvk_sdf_renderpass_postprocess_create(&renderpasses->postprocess, context, swapchain);

    return result;
}
nu_result_t nuvk_sdf_renderpasses_terminate(
    nuvk_sdf_renderpasses_t *renderpasses,
    const nuvk_context_t *context
)
{
    vkDestroyRenderPass(context->device, renderpasses->postprocess, &context->allocator);
    vkDestroyRenderPass(context->device, renderpasses->geometry, &context->allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderpasses_update_swapchain(
    nuvk_sdf_renderpasses_t *renderpasses,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_images_t *images
)
{
    vkDestroyRenderPass(context->device, renderpasses->postprocess, &context->allocator);
    vkDestroyRenderPass(context->device, renderpasses->geometry, &context->allocator);

    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_renderpass_geometry_create(&renderpasses->geometry, context, &images->geometry);
    result &= nuvk_sdf_renderpass_postprocess_create(&renderpasses->postprocess, context, swapchain);

    return result;
}