#include "command.h"

#include "../context/context.h"

nu_result_t nuvk_command_pool_create(VkCommandPool *command_pool)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    /* create the pool info */
    VkCommandPoolCreateInfo pool_info;
    memset(&pool_info, 0, sizeof(VkCommandPoolCreateInfo));
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = ctx->queue_family_indices.graphics_family;

    if (vkCreateCommandPool(ctx->device, &pool_info, NULL, command_pool) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_command_pool_destroy(VkCommandPool command_pool)
{
    const nuvk_context_t *ctx = nuvk_context_get();
    vkDestroyCommandPool(ctx->device, command_pool, NULL);
    return NU_SUCCESS;
}

nu_result_t nuvk_command_buffers_allocate(
    VkCommandBuffer *command_buffers,
    uint32_t command_buffer_count,
    VkCommandPool command_pool
)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    VkCommandBufferAllocateInfo alloc_info;
    memset(&alloc_info, 0, sizeof(VkCommandBufferAllocateInfo));
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = command_buffer_count;

    if (vkAllocateCommandBuffers(ctx->device, &alloc_info, command_buffers) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}