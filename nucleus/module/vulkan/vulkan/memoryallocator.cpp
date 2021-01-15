#include "memoryallocator.hpp"

#include "instance.hpp"
#include "../engine/engine.hpp"

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
        allocatorInfo.vulkanApiVersion = Instance::GetVulkanAPIVersion();
        allocatorInfo.instance         = instance.getInstance();
        allocatorInfo.physicalDevice   = physicalDevice.getPhysicalDevice();
        allocatorInfo.device           = device.getDevice();
        if (vmaCreateAllocator(&allocatorInfo, &allocator) != VK_SUCCESS) {
            Engine::Interrupt(MemoryAllocator::Section, "Failed to create allocator.");
        }
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

VmaAllocator MemoryAllocator::getAllocator() const
{
    return internal->allocator;
}

