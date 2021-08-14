#include <nucleus/module/vulkan/sdf/framebuffer/framebuffers.h>

nu_result_t nuvk_sdf_framebuffers_initialize(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_framebuffer_postprocess_create(&framebuffers->postprocess, context, swapchain, renderpasses->postprocess);

    return result;
}
nu_result_t nuvk_sdf_framebuffers_terminate(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context
)
{
    nuvk_sdf_framebuffer_postprocess_destroy(&framebuffers->postprocess, context);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_framebuffers_update_swapchain(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    nuvk_sdf_framebuffer_postprocess_destroy(&framebuffers->postprocess, context);
    return nuvk_sdf_framebuffer_postprocess_create(&framebuffers->postprocess, context, swapchain, renderpasses->postprocess);
}