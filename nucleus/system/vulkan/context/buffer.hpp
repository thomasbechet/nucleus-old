#pragma once

#include "../utility/internalptr.hpp"
#include "physicaldevice.hpp"
#include "device.hpp"
#include "commandpool.hpp"

namespace nuvk
{
    class Buffer
    {
    public:
        Buffer(
            const PhysicalDevice &physicalDevice,
            const Device &device,
            const vk::BufferUsageFlags &bufferFlags,
            const vk::MemoryPropertyFlags &memoryFlags,
            const vk::DeviceSize &size,
            const void *data
        );

        const vk::Buffer &getBuffer() const;

        static Buffer CreateDeviceLocalBuffer(
            const PhysicalDevice &physicalDevice,
            const Device &device,
            const CommandPool &commandPool,
            const vk::BufferUsageFlags &bufferFlags,
            const vk::DeviceSize &size,
            const void *data
        );

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}