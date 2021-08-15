#include <nucleus/module/vulkan/core/buffer.h>

nu_result_t nuvk_buffer_create(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager, const nuvk_buffer_info_t *info)
{
    VkBufferCreateInfo buffer_info;
    memset(&buffer_info, 0, sizeof(VkBufferCreateInfo));
    buffer_info.sType               = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size                = info->size;
    buffer_info.usage               = info->buffer_usage;
    buffer_info.sharingMode         = VK_SHARING_MODE_EXCLUSIVE;
    buffer_info.pQueueFamilyIndices = NULL;

    VmaAllocationCreateInfo allocation_info;
    memset(&allocation_info, 0, sizeof(VmaAllocationCreateInfo));
    allocation_info.usage = info->memory_usage;

    if (vmaCreateBuffer(manager->allocator, &buffer_info, &allocation_info, &buffer->buffer, &buffer->allocation, NULL) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create buffer.\n");
        return NU_SUCCESS;
    }

    buffer->map = NULL;

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_destroy(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager)
{
    if (buffer->map != NULL) {
        vmaUnmapMemory(manager->allocator, buffer->allocation);
    }
    vmaDestroyBuffer(manager->allocator, buffer->buffer, buffer->allocation);

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_map(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager)
{
    if (vmaMapMemory(manager->allocator, buffer->allocation, &buffer->map) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to map memory.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_buffer_unmap(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager)
{
    if (buffer->map != NULL) {
        vmaUnmapMemory(manager->allocator, buffer->allocation);
    }
    buffer->map = NULL;

    return NU_SUCCESS;
}