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
        static inline constexpr std::string_view Section = "BUFFER";

        Buffer(
            const PhysicalDevice &physicalDevice,
            const Device &device,
            VkBufferUsageFlags bufferFlags,
            VkMemoryPropertyFlags memoryFlags,
            VkDeviceSize size,
            const void *data
        );

        VkBuffer getBuffer() const;

        static Buffer CreateDeviceLocalBuffer(
            const PhysicalDevice &physicalDevice,
            const Device &device,
            const CommandPool &commandPool,
            VkBufferUsageFlags bufferFlags,
            VkDeviceSize size,
            const void *data
        );

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}