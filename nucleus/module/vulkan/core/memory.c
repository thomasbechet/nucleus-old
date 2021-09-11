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
        nu_error(NUVK_LOGGER_NAME, "Failed to create vma.");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_memory_manager_terminate(nuvk_memory_manager_t *manager)
{
    vmaDestroyAllocator(manager->allocator);

    return NU_SUCCESS;
}