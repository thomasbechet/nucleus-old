#include "buffer.hpp"

using namespace nuvk;

namespace
{
    static vk::UniqueBuffer CreateBuffer(
        const vk::Device &device,
        const vk::DeviceSize &size,
        const vk::BufferUsageFlags &bufferFlags
    )
    {
        vk::BufferCreateInfo info {
            vk::BufferCreateFlags(),
            size,
            bufferFlags,
            vk::SharingMode::eExclusive,
            0,
            nullptr
        };

        return device.createBufferUnique(info);
    }

    vk::UniqueDeviceMemory AllocateMemory(
        const PhysicalDevice &physicalDevice,
        const vk::Device &device,
        const vk::Buffer &buffer,
        const vk::MemoryPropertyFlags &memoryFlags
    )
    {
        vk::MemoryRequirements memoryRequirements {
            device.getBufferMemoryRequirements(buffer)
        };

        uint32_t memoryTypeIndex {
            physicalDevice.getMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryFlags)
        };

        vk::MemoryAllocateInfo info {
            memoryRequirements.size,
            memoryTypeIndex
        };

        return device.allocateMemoryUnique(info);
    }
}

struct Buffer::Internal
{
    vk::UniqueBuffer buffer;
    vk::UniqueDeviceMemory deviceMemory;

    Internal(
        const PhysicalDevice &physicalDevice,
        const Device &device,
        const vk::BufferUsageFlags &bufferFlags,
        const vk::MemoryPropertyFlags &memoryFlags,
        const vk::DeviceSize &size,
        const void *data
    )
    {
        buffer = ::CreateBuffer(device.getDevice(), size, bufferFlags);
        deviceMemory = ::AllocateMemory(physicalDevice, device.getDevice(), *buffer, memoryFlags);
        device.getDevice().bindBufferMemory(*buffer, *deviceMemory, 0);

        if (data) {
            void *mappedMemory {device.getDevice().mapMemory(deviceMemory.get(), 0, size)};
            std::memcpy(mappedMemory, data, static_cast<size_t>(size));
            device.getDevice().unmapMemory(deviceMemory.get());
        }
    }
    ~Internal()
    {

    }
};

Buffer::Buffer(
    const PhysicalDevice &physicalDevice,
    const Device &device,
    const vk::BufferUsageFlags &bufferFlags,
    const vk::MemoryPropertyFlags &memoryFlags,
    const vk::DeviceSize &size,
    const void *data
) : internal(MakeInternalPtr<Internal>(physicalDevice, device, bufferFlags, memoryFlags, size, data)) {}

const vk::Buffer &Buffer::getBuffer() const
{
    return *internal->buffer;
}

Buffer Buffer::CreateDeviceLocalBuffer(
    const PhysicalDevice &physicalDevice,
    const Device &device,
    const CommandPool &commandPool,
    const vk::BufferUsageFlags &bufferFlags,
    const vk::DeviceSize &size,
    const void *data
)
{
    Buffer stagingBuffer(
        physicalDevice,
        device,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent,
        size,
        data
    );

    Buffer deviceLocalBuffer(
        physicalDevice,
        device,
        vk::BufferUsageFlagBits::eTransferDst | bufferFlags,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        size,
        nullptr
    );

    vk::UniqueCommandBuffer commandBuffer = commandPool.beginCommandBuffer();
    vk::BufferCopy copyRegion(0, 0, size);
    commandBuffer->copyBuffer(stagingBuffer.getBuffer(), deviceLocalBuffer.getBuffer(), 1, &copyRegion);
    commandPool.endCommandBuffer(*commandBuffer, device.getGraphicsQueue());

    return deviceLocalBuffer;
}