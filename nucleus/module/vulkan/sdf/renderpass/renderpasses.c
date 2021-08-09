#include <nucleus/module/vulkan/sdf/renderpass/renderpasses.h>

#include <nucleus/module/vulkan/sdf/renderpass/postprocess.h>

nu_result_t nuvk_sdf_renderpasses_initialize(
    nuvk_sdf_renderpasses_t *renderpasses,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
)
{
    nu_result_t result = NU_SUCCESS;
    result &= nuvk_sdf_renderpass_postprocess_create(&renderpasses->postprocess, context, swapchain);

    return result;
}
nu_result_t nuvk_sdf_renderpasses_terminate(
    nuvk_sdf_renderpasses_t *renderpasses,
    const nuvk_context_t *context
)
{
    vkDestroyRenderPass(context->device, renderpasses->postprocess, &context->allocator);

    return NU_SUCCESS;
}