#pragma once

#include "../utility/internalptr.hpp"
#include "commandpool.hpp"
#include "memoryallocator.hpp"

namespace nuvk
{
    class Buffer
    {
    public:
        static inline constexpr std::string_view Section = "BUFFER";

        Buffer(
            const MemoryAllocator &allocator,
            VkBufferUsageFlags bufferUsage,
            VmaMemoryUsage memoryUsage,
            VkDeviceSize size,
            const void *data
        );

        VkBuffer getBuffer() const;

        static Buffer CreateDeviceLocalBuffer(
            const MemoryAllocator &allocator,
            const CommandPool &commandPool,
            VkQueue queue,
            VkBufferUsageFlags bufferUsage,
            VkDeviceSize size,
            const void *data
        );

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}