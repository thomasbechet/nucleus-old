#include <nucleus/module/vulkan/sdf/framebuffer/framebuffers.h>

nu_result_t nuvk_sdf_framebuffers_initialize(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_images_t *images,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    nu_result_t result;

    result = nuvk_sdf_framebuffer_geometry_create(&framebuffers->geometry, context, &images->geometry, renderpasses->geometry);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry framebuffer.");
    result = nuvk_sdf_framebuffer_postprocess_create(&framebuffers->postprocess, context, swapchain, renderpasses->postprocess);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create postprocess framebuffer.");

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_framebuffers_terminate(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context
)
{
    nuvk_sdf_framebuffer_postprocess_destroy(&framebuffers->postprocess, context);
    nuvk_sdf_framebuffer_geometry_destroy(&framebuffers->geometry, context);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_framebuffers_update_swapchain(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_images_t *images,
    const nuvk_sdf_renderpasses_t *renderpasses
)
{
    nuvk_sdf_framebuffer_postprocess_destroy(&framebuffers->postprocess, context);
    nuvk_sdf_framebuffer_geometry_destroy(&framebuffers->geometry, context);

    nu_result_t result;

    result = nuvk_sdf_framebuffer_geometry_create(&framebuffers->geometry, context, &images->geometry, renderpasses->geometry);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update geometry framebuffer.");
    result = nuvk_sdf_framebuffer_postprocess_create(&framebuffers->postprocess, context, swapchain, renderpasses->postprocess);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update postprocess framebuffer.");


    return result;
}