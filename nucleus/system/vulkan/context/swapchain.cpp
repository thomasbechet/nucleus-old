#include "swapchain.hpp"

using namespace nuvk;

SwapChainSupportDetails Swapchain::QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}