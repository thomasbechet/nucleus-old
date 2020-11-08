#pragma once

#include "../utility/internalptr.hpp"
#include "instance.hpp"
#include "device.hpp"
#include "physicaldevice.hpp"

namespace nuvk
{
    class MemoryAllocator
    {
    public:
        MemoryAllocator(
            const Instance &instance,
            const PhysicalDevice &physicalDevice,
            const Device &device
        );

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}