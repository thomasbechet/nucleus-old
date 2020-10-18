#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    class Swapchain
    {
    public:
        inline constexpr auto Section = "SWAPCHAIN";

        Swapchain(Context &context);

        static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface);        
    
    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}