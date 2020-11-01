#include "swapchain.hpp"

#include "../engine/engine.hpp"
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
        const vk::Device &device,
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
            vk::UniqueSwapchainKHR swapChain = device.createSwapchainKHRUnique(createInfo);

            swapChainImages = device.getSwapchainImagesKHR(*swapChain);
            swapChainImageFormat = surfaceFormat.format;
            swapChainExtent = extent;

            return swapChain;
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create swapchain.");
        }

        throw std::runtime_error("Unknown error.");
    }
    static std::vector<vk::UniqueImageView> CreateImageViews(
        const vk::Device &device,
        std::vector<vk::Image> &swapchainImages,
        vk::Format &swapchainImageFormat
    )
    {
        std::vector<vk::UniqueImageView> swapChainImageViews;
        swapChainImageViews.resize(swapchainImages.size());

        for (uint32_t i = 0; i < swapchainImages.size(); i++) {
            vk::ImageViewCreateInfo createInfo = {};
            createInfo.image = swapchainImages[i];
            createInfo.viewType = vk::ImageViewType::e2D;
            createInfo.format = swapchainImageFormat;
            createInfo.components.r = vk::ComponentSwizzle::eIdentity;
            createInfo.components.g = vk::ComponentSwizzle::eIdentity;
            createInfo.components.b = vk::ComponentSwizzle::eIdentity;
            createInfo.components.a = vk::ComponentSwizzle::eIdentity;
            createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            try {
                swapChainImageViews[i] = device.createImageViewUnique(createInfo);
            } catch (vk::SystemError &err) {
                Engine::Interrupt("Failed to create image views.");
            }
        }

        return swapChainImageViews;
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
        const Device &device,
        const PhysicalDevice &physicalDevice,
        const Surface &surface,
        uint32_t width, uint32_t height
    )
    {
        swapchain = ::CreateSwapchain(
            device.getDevice(), 
            physicalDevice.getPhysicalDevice(), 
            surface.getSurface(), 
            width, height, 
            swapchainImages, 
            swapchainImageFormat,
            swapchainExtent
        );
        swapchainImageViews = ::CreateImageViews(
            device.getDevice(), 
            swapchainImages, 
            swapchainImageFormat
        );
    }
    ~Internal()
    {

    }
};

Swapchain::Swapchain(
    const Device &device,
    const PhysicalDevice &physicalDevice,
    const Surface &surface,
    uint32_t width, uint32_t height
) : internal(MakeInternalPtr<Internal>(device, physicalDevice, surface, width, height)) {}

vk::Format Swapchain::getFormat() const
{
    return internal->swapchainImageFormat;
}
vk::Extent2D Swapchain::getExtent() const
{
    return internal->swapchainExtent;
}
std::vector<vk::ImageView> Swapchain::getImageViews() const
{
    std::vector<vk::ImageView> imageViews;
    for (auto &imageView : internal->swapchainImageViews) {
        imageViews.push_back(*imageView);
    }
    return imageViews;
}
const vk::SwapchainKHR &Swapchain::getSwapchain() const
{
    return *internal->swapchain;
}

SwapChainSupportDetails Swapchain::QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}