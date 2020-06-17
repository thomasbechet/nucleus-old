#include "render_pass.h"

#include "../common/logger.h"
#include "../presentation/swapchain.h"
#include "../presentation/device.h"

typedef struct {
    VkRenderPass render_pass;
} nuvk_render_pass_data_t;

static nuvk_render_pass_data_t _data;

nu_result_t nuvk_render_pass_create(void)
{
    /* color attachment */
    VkAttachmentDescription color_attachment;
    memset(&color_attachment, 0, sizeof(VkAttachmentDescription));
    color_attachment.format = nuvk_swapchain_get_format();
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    /* color attachment reference */
    VkAttachmentReference color_attachment_ref;
    memset(&color_attachment_ref, 0, sizeof(VkAttachmentReference));
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    /* subpass */
    VkSubpassDescription subpass;
    memset(&subpass, 0, sizeof(VkSubpassDescription));
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    /* create render pass */
    VkRenderPassCreateInfo render_pass_info;
    memset(&render_pass_info, 0, sizeof(VkRenderPassCreateInfo));
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;

    if (vkCreateRenderPass(nuvk_device_get_handle(), &render_pass_info, NULL, &_data.render_pass) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to create render pass.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_render_pass_destroy(void)
{
    vkDestroyRenderPass(nuvk_device_get_handle(), _data.render_pass, NULL);

    return NU_SUCCESS;
}

VkRenderPass nuvk_render_pass_get_handle(void)
{
    return _data.render_pass;
}