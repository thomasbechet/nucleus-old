#pragma once

#include "../utility/internalptr.hpp"
#include "instance.hpp"
#include "device.hpp"
#include "physicaldevice.hpp"

#include <vma/vk_mem_alloc.h>

namespace nuvk
{
    class MemoryAllocator
    {
    public:
        static inline constexpr std::string_view Section = "MEMORYALLOCATOR";

        MemoryAllocator(
            const Instance &instance,
            const PhysicalDevice &physicalDevice,
            const Device &device
        );

        VmaAllocator getAllocator() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}