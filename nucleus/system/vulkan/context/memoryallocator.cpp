#include "memoryallocator.hpp"

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

using namespace nuvk;

namespace
{

}

struct MemoryAllocator::Internal
{
    VmaAllocator allocator;

    Internal(
        const Instance &instance,
        const PhysicalDevice &physicalDevice,
        const Device &device
    )
    {
        VmaAllocatorCreateInfo allocatorInfo{};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
        allocatorInfo.instance         = instance.getInstance();
        allocatorInfo.physicalDevice   = physicalDevice.getPhysicalDevice();
        allocatorInfo.device           = device.getDevice();
        vmaCreateAllocator(&allocatorInfo, &allocator);
    }
    ~Internal()
    {
        vmaDestroyAllocator(allocator);
    }
};

MemoryAllocator::MemoryAllocator(
    const Instance &instance,
    const PhysicalDevice &physicalDevice,
    const Device &device
) : internal(MakeInternalPtr<Internal>(instance, physicalDevice, device)) {}

