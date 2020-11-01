#pragma once

#include "../utility/internalptr.hpp"
#include "device.hpp"
#include "physicaldevice.hpp"
#include "surface.hpp"

#include <vulkan/vulkan.hpp>

#include <vector>

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
        static inline constexpr std::string_view Section = "SWAPCHAIN";

        Swapchain(
            const Device &device,
            const PhysicalDevice &physicalDevice,
            const Surface &surface,
            uint32_t width, uint32_t height
        );

        vk::Format getFormat() const;
        vk::Extent2D getExtent() const;
        std::vector<vk::ImageView> getImageViews() const;
        const vk::SwapchainKHR &getSwapchain() const;

        static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface);        
    
    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}