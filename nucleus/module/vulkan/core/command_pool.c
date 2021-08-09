#include <nucleus/module/vulkan/core/command_pool.h>

nu_result_t nuvk_command_pool_initialize(
    nuvk_command_pool_t *pool, 
    const nuvk_context_t *context
)
{
    VkCommandPoolCreateInfo info;
    memset(&info, 0, sizeof(VkCommandPoolCreateInfo));
    info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = context->graphics_queue_family_index;

    if (vkCreateCommandPool(context->device, &info, &context->allocator, &pool->graphics_command_pool) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create command pool.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_command_pool_terminate(nuvk_command_pool_t *pool, const nuvk_context_t *context)
{
    vkDestroyCommandPool(context->device, pool->graphics_command_pool, &context->allocator);

    return NU_SUCCESS;
}

nu_result_t nuvk_command_buffer_begin_single(VkDevice device, VkCommandPool command_pool, VkCommandBuffer *command_buffer)
{
    VkCommandBufferAllocateInfo allocate_info;
    memset(&allocate_info, 0, sizeof(VkCommandBufferAllocateInfo));
    allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool        = command_pool;
    allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &allocate_info, command_buffer) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to allocate single command buffer.\n");
        return NU_FAILURE;
    }

    VkCommandBufferBeginInfo begin_info;
    memset(&begin_info, 0, sizeof(VkCommandBufferBeginInfo));
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(*command_buffer, &begin_info) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to begin single command buffer.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_command_buffer_end_single(VkCommandBuffer command_buffer, VkCommandPool command_pool, VkQueue queue)
{
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo info;
    memset(&info, 0, sizeof(VkSubmitInfo));
    info.sType           = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(queue, 1, &info, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);

    return NU_SUCCESS;
}
nu_result_t nuvk_command_buffer_create(VkDevice device, VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers)
{
    VkCommandBufferAllocateInfo info;
    memset(&info, 0, sizeof(VkCommandBufferAllocateInfo));
    info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool        = pool;
    info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = count;

    if (vkAllocateCommandBuffers(device, &info, command_buffers) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to allocate command buffers.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}