#include <nucleus/module/vulkan/renderpass/postprocess.h>

nu_result_t nuvk_renderpass_postprocess_create(
    nuvk_renderpass_t *renderpass,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
)
{
    VkAttachmentDescription attachment;
    memset(&attachment, 0, sizeof(VkAttachmentDescription));
    attachment.format         = swapchain->surface_color_format;
    attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_reference;
    memset(&color_reference, 0, sizeof(VkAttachmentReference));
    color_reference.attachment = 0;
    color_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass;
    memset(&subpass, 0, sizeof(VkSubpassDescription));
    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments    = &color_reference;

    VkRenderPassCreateInfo info;
    memset(&info, 0, sizeof(VkRenderPassCreateInfo));
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments    = &attachment;
    info.subpassCount    = 1;
    info.pSubpasses      = &subpass;

    return nuvk_renderpass_create(renderpass, context, &info);
}