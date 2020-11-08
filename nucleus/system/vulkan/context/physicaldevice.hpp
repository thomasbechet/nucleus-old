#pragma once

#include "../utility/internalptr.hpp"
#include "instance.hpp"
#include "surface.hpp"

#include <vulkan/vulkan.h>

#include <optional>

namespace nuvk
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class PhysicalDevice
    {
    public:
        static inline constexpr std::string_view Section = "PHYSICALDEVICE";

        PhysicalDevice(
            const Instance &instance,
            const Surface &surface
        );

        VkPhysicalDevice getPhysicalDevice() const;
        uint32_t getMemoryTypeIndex(
            uint32_t filter,
            VkMemoryPropertyFlags flags
        ) const;
        
        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}