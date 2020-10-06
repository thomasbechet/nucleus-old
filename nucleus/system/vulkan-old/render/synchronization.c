#include "synchronization.h"

#include "../context/context.h"

nu_result_t nuvk_semaphore_create(VkSemaphore *semaphore)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    VkSemaphoreCreateInfo semaphore_info;
    memset(&semaphore_info, 0, sizeof(VkSemaphoreCreateInfo));
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(ctx->device, &semaphore_info, NULL, semaphore) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_semaphore_destroy(VkSemaphore semaphore)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    vkDestroySemaphore(ctx->device, semaphore, NULL);

    return NU_SUCCESS;
}
nu_result_t nuvk_fence_create(VkFence *fence)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    VkFenceCreateInfo fence_info;
    memset(&fence_info, 0, sizeof(VkFenceCreateInfo));
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(ctx->device, &fence_info, NULL, fence) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_fence_destroy(VkFence fence)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    vkDestroyFence(ctx->device, fence, NULL);

    return NU_SUCCESS;
}