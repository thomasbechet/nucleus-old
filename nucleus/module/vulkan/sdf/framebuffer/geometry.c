#include <nucleus/module/vulkan/sdf/framebuffer/geometry.h>

nu_result_t nuvk_sdf_framebuffer_geometry_create(
    nuvk_sdf_framebuffer_geometry_t *framebuffer,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *image,
    VkRenderPass geometry_renderpass
)
{
    VkFramebufferCreateInfo info;
    memset(&info, 0, sizeof(VkFramebufferCreateInfo));
    info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.width           = image->image.extent.width;
    info.height          = image->image.extent.height;
    info.layers          = 1;
    info.renderPass      = geometry_renderpass;
    info.attachmentCount = 1;
    info.pAttachments    = &image->image.image_view;

    if (vkCreateFramebuffer(context->device, &info, &context->allocator, &framebuffer->framebuffer) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to create geometry framebuffer.");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}   
nu_result_t nuvk_sdf_framebuffer_geometry_destroy(
    nuvk_sdf_framebuffer_geometry_t *framebuffer,
    const nuvk_context_t *context
)
{
    vkDestroyFramebuffer(context->device, framebuffer->framebuffer, &context->allocator);
    
    return NU_SUCCESS;
}