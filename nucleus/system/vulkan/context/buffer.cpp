#include "buffer.hpp"

#include "../engine/engine.hpp"

#include <cstring>

using namespace nuvk;

namespace
{
    static VkResult CreateBufferAndMemory(
        VmaAllocator allocator,
        VkDeviceSize size,
        VkBufferUsageFlags bufferUsage,
        VmaMemoryUsage memoryUsage,
        VkBuffer &buffer,
        VmaAllocation &allocation
    )
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size                  = size;
        bufferInfo.usage                 = bufferUsage;
        bufferInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
        bufferInfo.queueFamilyIndexCount = 0;
        bufferInfo.pQueueFamilyIndices   = nullptr;

        VmaAllocationCreateInfo allocationInfo{};
        allocationInfo.usage = memoryUsage;

        return vmaCreateBuffer(allocator, &bufferInfo, &allocationInfo, &buffer, &allocation, nullptr);
    }
}

struct Buffer::Internal
{
    VmaAllocator allocator;
    VkBuffer buffer;
    VmaAllocation allocation;

    Internal(
        const MemoryAllocator &allocator,
        VkBufferUsageFlags bufferUsage,
        VmaMemoryUsage memoryUsage,
        VkDeviceSize size,
        const void *data
    ) : allocator(allocator.getAllocator())
    {   
        if (::CreateBufferAndMemory(allocator.getAllocator(), size, bufferUsage, memoryUsage, buffer, allocation) != VK_SUCCESS) {
            Engine::Interrupt(Buffer::Section, "Failed to create buffer.");
        }

        if (data) {
            void *mappedMemory;
            vmaMapMemory(allocator.getAllocator(), allocation, &mappedMemory);
            std::memcpy(mappedMemory, data, static_cast<size_t>(size));
            vmaUnmapMemory(allocator.getAllocator(), allocation);
        }
    }
    ~Internal()
    {
        vmaDestroyBuffer(allocator, buffer, allocation);
    }
};

Buffer::Buffer(
    const MemoryAllocator &allocator,
    VkBufferUsageFlags bufferUsage,
    VmaMemoryUsage memoryUsage,
    VkDeviceSize size,
    const void *data
) : internal(MakeInternalPtr<Internal>(allocator, bufferUsage, memoryUsage, size, data)) {}

VkBuffer Buffer::getBuffer() const
{
    return internal->buffer;
}

Buffer Buffer::CreateDeviceLocalBuffer(
    const MemoryAllocator &allocator,
    const CommandPool &commandPool,
    VkQueue queue,
    VkBufferUsageFlags bufferUsage,
    VkDeviceSize size,
    const void *data
)
{
    Buffer stagingBuffer(
        allocator,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_ONLY,
        size,
        data
    );

    Buffer deviceLocalBuffer(
        allocator,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferUsage,
        VMA_MEMORY_USAGE_GPU_ONLY,
        size,
        nullptr
    );

    VkCommandBuffer commandBuffer = commandPool.beginCommandBuffer();

    VkBufferCopy copyRegion;
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size      = size;
    vkCmdCopyBuffer(commandBuffer, stagingBuffer.getBuffer(), deviceLocalBuffer.getBuffer(), 1, &copyRegion);

    commandPool.endCommandBuffer(commandBuffer, queue);

    return deviceLocalBuffer;
}