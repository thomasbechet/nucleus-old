#include "buffer.hpp"

#include "../engine/engine.hpp"

#include <cstring>

using namespace nuvk;

namespace
{
    static VkBuffer CreateBuffer(
        VkDevice device,
        VkDeviceSize size,
        VkBufferUsageFlags bufferFlags
    )
    {
        VkBufferCreateInfo info{};
        info.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        info.size                  = size;
        info.usage                 = bufferFlags;
        info.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
        info.queueFamilyIndexCount = 0;
        info.pQueueFamilyIndices   = nullptr;

        VkBuffer buffer;
        if (vkCreateBuffer(device, &info, NULL, &buffer) != VK_SUCCESS) {
            Engine::Interrupt(Buffer::Section, "Failed to create buffer.");
        }
        return buffer;
    }

    static VkDeviceMemory AllocateMemory(
        const PhysicalDevice &physicalDevice,
        VkDevice device,
        VkBuffer buffer,
        VkMemoryPropertyFlags memoryFlags
    )
    {
        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

        VkMemoryAllocateInfo info{};
        info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.allocationSize  = memoryRequirements.size;
        info.memoryTypeIndex = physicalDevice.getMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryFlags);

        VkDeviceMemory deviceMemory;
        if (vkAllocateMemory(device, &info, NULL, &deviceMemory) != VK_SUCCESS) {
            Engine::Interrupt(Buffer::Section, "Failed to allocate memory.");
        }
        return deviceMemory;
    }
}

struct Buffer::Internal
{
    VkDevice device;
    VkBuffer buffer;
    VkDeviceMemory deviceMemory;

    Internal(
        const PhysicalDevice &physicalDevice,
        const Device &device,
        VkBufferUsageFlags bufferFlags,
        VkMemoryPropertyFlags memoryFlags,
        VkDeviceSize size,
        const void *data
    ) : device(device.getDevice())
    {   
        buffer = ::CreateBuffer(device.getDevice(), size, bufferFlags);
        deviceMemory = ::AllocateMemory(physicalDevice, device.getDevice(), buffer, memoryFlags);

        vkBindBufferMemory(device.getDevice(), buffer, deviceMemory, 0);

        if (data) {
            void *mappedMemory;
            vkMapMemory(device.getDevice(), deviceMemory, 0, size, 0, &mappedMemory);
            std::memcpy(mappedMemory, data, static_cast<size_t>(size));
            vkUnmapMemory(device.getDevice(), deviceMemory);
        }
    }
    ~Internal()
    {
        vkFreeMemory(device, deviceMemory, nullptr);
        vkDestroyBuffer(device, buffer, nullptr);
    }
};

Buffer::Buffer(
    const PhysicalDevice &physicalDevice,
    const Device &device,
    VkBufferUsageFlags bufferFlags,
    VkMemoryPropertyFlags memoryFlags,
    VkDeviceSize size,
    const void *data
) : internal(MakeInternalPtr<Internal>(physicalDevice, device, bufferFlags, memoryFlags, size, data)) {}

VkBuffer Buffer::getBuffer() const
{
    return internal->buffer;
}

Buffer Buffer::CreateDeviceLocalBuffer(
    const PhysicalDevice &physicalDevice,
    const Device &device,
    const CommandPool &commandPool,
    VkBufferUsageFlags bufferFlags,
    VkDeviceSize size,
    const void *data
)
{
    Buffer stagingBuffer(
        physicalDevice,
        device,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        size,
        data
    );

    Buffer deviceLocalBuffer(
        physicalDevice,
        device,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferFlags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        size,
        nullptr
    );

    VkCommandBuffer commandBuffer = commandPool.beginCommandBuffer();

    VkBufferCopy copyRegion;
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size      = size;
    vkCmdCopyBuffer(commandBuffer, stagingBuffer.getBuffer(), deviceLocalBuffer.getBuffer(), 1, &copyRegion);

    commandPool.endCommandBuffer(commandBuffer, device.getGraphicsQueue());

    return deviceLocalBuffer;
}