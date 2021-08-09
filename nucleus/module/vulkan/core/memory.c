#include <nucleus/module/vulkan/core/memory.h>

nu_result_t nuvk_memory_manager_initialize(
    nuvk_memory_manager_t *manager, 
    const nuvk_context_t *context
)
{
    VmaAllocatorCreateInfo info;
    memset(&info, 0, sizeof(VmaAllocatorCreateInfo));
    info.vulkanApiVersion     = NUVK_CONTEXT_API_VERSION;
    info.instance             = context->instance;
    info.physicalDevice       = context->physical_device;
    info.device               = context->device;
    info.pAllocationCallbacks = &context->allocator;

    if (vmaCreateAllocator(&info, &manager->allocator) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create vma.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_memory_manager_terminate(nuvk_memory_manager_t *manager)
{
    vmaDestroyAllocator(manager->allocator);

    return NU_SUCCESS;
}

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