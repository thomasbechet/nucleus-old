#include "synchronization.h"

#include "../common/logger.h"
#include "../presentation/device.h"

nu_result_t nuvk_semaphore_create(VkSemaphore *semaphore)
{
    VkSemaphoreCreateInfo semaphore_info;
    memset(&semaphore_info, 0, sizeof(VkSemaphoreCreateInfo));
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(nuvk_device_get_handle(), &semaphore_info, NULL, semaphore) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_semaphore_destroy(VkSemaphore semaphore)
{
    vkDestroySemaphore(nuvk_device_get_handle(), semaphore, NULL);

    return NU_SUCCESS;
}
nu_result_t nuvk_fence_create(VkFence *fence)
{
    VkFenceCreateInfo fence_info;
    memset(&fence_info, 0, sizeof(VkFenceCreateInfo));
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(nuvk_device_get_handle(), &fence_info, NULL, fence) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_fence_destroy(VkFence fence)
{
    vkDestroyFence(nuvk_device_get_handle(), fence, NULL);

    return NU_SUCCESS;
}