#pragma once

#include "../utility/internalptr.hpp"

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
            const vk::Device &device,
            vk::PhysicalDevice physicalDevice,
            VkSurfaceKHR surface,
            uint32_t width, uint32_t height
        );

        vk::Format getFormat();
        vk::Extent2D getExtent();
        std::vector<vk::ImageView> getImageViews() const;

        static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface);        
    
    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}