#include <nucleus/module/vulkan/sdf/framebuffer/postprocess.h>

nu_result_t nuvk_sdf_framebuffer_postprocess_create(
    nuvk_sdf_framebuffer_postprocess_t *framebuffer,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    VkRenderPass postprocess_renderpass
)
{
    framebuffer->framebuffer_count = swapchain->image_count;
    framebuffer->framebuffers      = (VkFramebuffer*)nu_malloc(sizeof(VkFramebuffer) * framebuffer->framebuffer_count);

    for (uint32_t i = 0; i < framebuffer->framebuffer_count; i++) {
        VkFramebufferCreateInfo info;
        memset(&info, 0, sizeof(VkFramebufferCreateInfo));
        info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.width           = swapchain->extent.width;
        info.height          = swapchain->extent.height;
        info.layers          = 1;
        info.renderPass      = postprocess_renderpass;
        info.attachmentCount = 1;
        info.pAttachments    = &swapchain->image_views[i];
    
        if (vkCreateFramebuffer(context->device, &info, &context->allocator, &framebuffer->framebuffers[i]) != VK_SUCCESS) {
            nu_error(NUVK_LOGGER_NAME, "Failed to create postprocess framebuffer.");
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_framebuffer_postprocess_destroy(
    nuvk_sdf_framebuffer_postprocess_t *framebuffer,
    const nuvk_context_t *context
)
{
    for (uint32_t i = 0; i < framebuffer->framebuffer_count; i++) {
        vkDestroyFramebuffer(context->device, framebuffer->framebuffers[i], &context->allocator);
    }

    nu_free(framebuffer->framebuffers);

    return NU_SUCCESS;
}