#include "swapchain.hpp"

#include "engine.hpp"
#include "physicaldevice.hpp"

using namespace nuvk;

namespace
{
    static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
    {
        if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
            return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
        }

        for (const auto &availableFormat : availableFormats) {
            if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }
    static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes)
    {
        vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

        for (const auto &availablePresentMode : availablePresentModes) {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                return availablePresentMode;
            } else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
                bestMode = availablePresentMode;
            }
        }

        return bestMode;
    }
    static vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            vk::Extent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
            return actualExtent;
        }
    }

    static vk::UniqueSwapchainKHR CreateSwapchain(
        vk::UniqueDevice &device,
        vk::PhysicalDevice physicalDevice,
        VkSurfaceKHR surface,
        uint32_t width, uint32_t height,

        std::vector<vk::Image> &swapChainImages,
        vk::Format &swapChainImageFormat,
        vk::Extent2D &swapChainExtent
    )
    {
        SwapChainSupportDetails swapChainSupport = Swapchain::QuerySwapChainSupport(physicalDevice, surface);

        vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, width, height);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR createInfo(
            vk::SwapchainCreateFlagsKHR(),
            surface,
            imageCount,
            surfaceFormat.format,
            surfaceFormat.colorSpace,
            extent,
            1, // imageArrayLayers
            vk::ImageUsageFlagBits::eColorAttachment
        );

        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(physicalDevice, surface);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

        try {
            vk::UniqueSwapchainKHR swapChain = device->createSwapchainKHRUnique(createInfo);

            swapChainImages = device->getSwapchainImagesKHR(*swapChain);
            swapChainImageFormat = surfaceFormat.format;
            swapChainExtent = extent;

            return swapChain;
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create swapchain.");
        }

        throw std::runtime_error("Unknown error.");
    }
    static void CreateImageViews()
    {

    }
}

struct Swapchain::Internal
{
    vk::UniqueSwapchainKHR swapchain;
    std::vector<vk::Image> swapchainImages;
    vk::Format swapchainImageFormat;
    vk::Extent2D swapchainExtent;
    
    std::vector<vk::UniqueImageView> swapchainImageViews;
    std::vector<vk::UniqueFramebuffer> swapchainFramebuffers;

    Internal(
        vk::UniqueDevice &device,
        vk::PhysicalDevice physicalDevice,
        VkSurfaceKHR surface,
        uint32_t width, uint32_t height
    )
    {
        swapchain = ::CreateSwapchain(device, physicalDevice, surface, width, height, 
            swapchainImages, swapchainImageFormat, swapchainExtent);
    }
    ~Internal()
    {

    }
};

Swapchain::Swapchain(
    vk::UniqueDevice &device,
    vk::PhysicalDevice physicalDevice,
    VkSurfaceKHR surface,
    uint32_t width, uint32_t height
) : internal(MakeInternalPtr<Internal>(device, physicalDevice, surface, width, height)) {}

SwapChainSupportDetails Swapchain::QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}