#pragma once

#include "../utility/internalptr.hpp"
#include "device.hpp"
#include "physicaldevice.hpp"
#include "surface.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace nuvk
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class Swapchain
    {
    public:
        static inline constexpr std::string_view Section = "SWAPCHAIN";

        Swapchain(
            const Device &device,
            const PhysicalDevice &physicalDevice,
            const Surface &surface,
            uint32_t width, uint32_t height
        );

        VkFormat getFormat() const;
        VkExtent2D getExtent() const;
        std::vector<VkImageView> getImageViews() const;
        VkSwapchainKHR getSwapchain() const;

        static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);        
    
    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}