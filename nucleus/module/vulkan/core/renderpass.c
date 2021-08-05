#include <nucleus/module/vulkan/core/renderpass.h>

nu_result_t nuvk_renderpass_create(
    nuvk_renderpass_t *renderpass,
    const nuvk_context_t *context,
    const VkRenderPassCreateInfo *info
)
{
    if (vkCreateRenderPass(context->device, info, &context->allocator, &renderpass->renderpass) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_renderpass_destroy(
    nuvk_renderpass_t *renderpass,
    const nuvk_context_t *context
)
{
    vkDestroyRenderPass(context->device, renderpass->renderpass, &context->allocator);

    return NU_SUCCESS;
}