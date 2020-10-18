#pragma once

#include "../utility/internalptr.hpp"

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
        inline constexpr auto Section = "PHYSICALDEVICE";

        PhysicalDevice(
            vk::UniqueInstance &instance,
            vk::SurfaceKHR &surface
        );
        
        static IsSuitable(vk::PhysicalDevice &device, vk::SurfaceKHR &surface);
        static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface);

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}