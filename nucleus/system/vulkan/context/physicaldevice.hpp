#pragma once

#include "../utility/internalptr.hpp"
#include "instance.hpp"
#include "surface.hpp"

#include <vulkan/vulkan.hpp>

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

        vk::PhysicalDevice getPhysicalDevice() const;
        uint32_t getMemoryTypeIndex(
            const uint32_t &filter,
            const vk::MemoryPropertyFlags &flags
        ) const;
        
        static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface);

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}