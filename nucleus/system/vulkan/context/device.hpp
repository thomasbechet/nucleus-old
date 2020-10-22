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

        const vk::Device &getDevice() const;
        const vk::Queue &getGraphicsQueue() const;
        uint32_t getGraphicsQueueIndex() const;
        const vk::Queue &getPresentQueue() const;
        uint32_t getPresentQueueIndex() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}