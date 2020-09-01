#include "framebuffer.h"

#include "../context/context.h"

nu_result_t nuvk_framebuffer_create(
    VkFramebuffer *framebuffer,
    VkImageView *images,
    uint32_t image_count,
    VkExtent2D extent,
    VkRenderPass renderpass
)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    VkFramebufferCreateInfo framebuffer_info;
    memset(&framebuffer_info, 0, sizeof(VkFramebufferCreateInfo));
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = renderpass;
    framebuffer_info.pAttachments = images;
    framebuffer_info.attachmentCount = image_count;
    framebuffer_info.width = extent.width;
    framebuffer_info.height = extent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer(ctx->device, &framebuffer_info, NULL, framebuffer) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_framebuffer_destroy(VkFramebuffer framebuffer)
{
    const nuvk_context_t *ctx = nuvk_context_get();
    vkDestroyFramebuffer(ctx->device, framebuffer, NULL);
    return NU_SUCCESS;
}