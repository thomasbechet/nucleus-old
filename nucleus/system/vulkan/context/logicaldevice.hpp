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

        LogicalDevice(
            vk::PhysicalDevice &physicalDevice,
            VkSurfaceKHR &surface,
            bool useValidationLayers
        );

        vk::UniqueDevice &getLogicalDevice();
        vk::Queue &getGraphicsQueue();
        vk::Queue &getPresentQueue();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}