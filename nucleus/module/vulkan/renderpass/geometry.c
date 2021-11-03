#include <nucleus/module/vulkan/renderpass/geometry.h>

nu_result_t nuvk_renderpass_geometry_create(
    VkRenderPass *renderpass,
    const nuvk_context_t *context,
    const nuvk_image_geometry_t *image
)
{
    VkAttachmentDescription attachments[2];
    memset(attachments, 0, sizeof(VkAttachmentDescription) * 2);

    attachments[0].format         = image->normal_depth.format;
    attachments[0].samples        = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;

    attachments[1].format         = image->position_material.format;
    attachments[1].samples        = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;

    VkAttachmentReference color_references[2];
    memset(color_references, 0, sizeof(VkAttachmentReference) * 2);
    color_references[0].attachment = 0;
    color_references[0].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    color_references[1].attachment = 1;
    color_references[1].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass;
    memset(&subpass, 0, sizeof(VkSubpassDescription));
    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 2;
    subpass.pColorAttachments    = color_references;

    VkRenderPassCreateInfo info;
    memset(&info, 0, sizeof(VkRenderPassCreateInfo));
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 2;
    info.pAttachments    = attachments;
    info.subpassCount    = 1;
    info.pSubpasses      = &subpass;

    VkResult result = vkCreateRenderPass(context->device, &info, &context->allocator, renderpass);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry renderpass.");

    return NU_SUCCESS;
}