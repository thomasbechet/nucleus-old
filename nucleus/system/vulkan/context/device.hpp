#pragma once

#include "../utility/internalptr.hpp"
#include "physicaldevice.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class Device
    {
    public:
        static inline constexpr std::string_view Section = "DEVICE";

        Device(
            vk::PhysicalDevice physicalDevice,
            VkSurfaceKHR surface,
            bool useValidationLayers
        );

        vk::Device &getDevice();
        vk::Queue &getGraphicsQueue();
        vk::Queue &getPresentQueue();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}