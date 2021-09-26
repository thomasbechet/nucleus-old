#include <nucleus/module/vulkan/sdf/framebuffer/geometry.h>

nu_result_t nuvk_sdf_framebuffer_geometry_create(
    nuvk_sdf_framebuffer_geometry_t *framebuffer,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *image,
    VkRenderPass geometry_renderpass
)
{
    VkImageView attachments[2] = {
        image->normal_depth.image_view,
        image->position_material.image_view
    }; 

    VkFramebufferCreateInfo info;
    memset(&info, 0, sizeof(VkFramebufferCreateInfo));
    info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.width           = image->normal_depth.extent.width;
    info.height          = image->normal_depth.extent.height;
    info.layers          = 1;
    info.renderPass      = geometry_renderpass;
    info.attachmentCount = 2;
    info.pAttachments    = attachments;

    VkResult result = vkCreateFramebuffer(context->device, &info, &context->allocator, &framebuffer->framebuffer);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry framebuffer.");

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