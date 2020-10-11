#pragma once

#include "context.hpp"
#include "../utility/loggable.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    class Swapchain : public Loggable
    {
    public:
        static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface);
    
    public:
        Swapchain(Context &context);
        ~Swapchain();

    public:
        void createSwapchain();
        void createImageViews();
    
    private:
        vk::UniqueSwapchainKHR m_swapChain;
        std::vector<vk::Image> m_swapChainImages;
        vk::Format m_swapChainImageFormat;
        vk::Extent2D m_swapChainExtent;
        std::vector<vk::UniqueImageView> m_swapChainImageViews;
        std::vector<vk::UniqueFramebuffer> m_swapChainFramebuffers;

        Context &m_context;
    };
}