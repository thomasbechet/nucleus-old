#pragma once

#include "../utility/internalptr.hpp"
#include "physicaldevice.hpp"
#include "surface.hpp"

#include <vulkan/vulkan.h>

namespace nuvk
{
    class Device
    {
    public:
        static inline constexpr std::string_view Section = "DEVICE";

        Device(
            const PhysicalDevice &physicalDevice,
            const Surface &surface,
            bool useValidationLayers
        );

        VkDevice getDevice() const;
        VkQueue getGraphicsQueue() const;
        uint32_t getGraphicsQueueIndex() const;
        VkQueue getPresentQueue() const;
        uint32_t getPresentQueueIndex() const;

        static std::vector<const char*> GetRequiredValidationLayers();
        static std::vector<const char*> GetRequiredDeviceExtensions();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}