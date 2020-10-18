#pragma once

#include "../utility/internalptr.hpp"
#include "physicaldevice.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class LogicalDevice
    {
    public:
        inline constexpr auto Section = "LOGICALDEVICE";

        LogicalDevice(PhysicalDevice &physicalDevice);

        vk::Queue &getGraphicsQueue();
        vk::Queue &getPresentQueue();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}