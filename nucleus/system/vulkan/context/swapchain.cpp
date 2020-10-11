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

Swapchain::Swapchain(Context &context) : 
    Loggable("SWAPCHAIN"),
    m_context(context)
{

}
Swapchain::~Swapchain()
{

}

void Swapchain::createSwapchain()
{
    SwapChainSupportDetails swapChainSupport = Engine::QuerySwapChainSupport(m_physicalDevice, m_surface);

    vk::SurfaceFormatKHR surfaceFormat = Engine::ChooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = Engine::ChooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = Engine::ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo(
        vk::SwapchainCreateFlagsKHR(),
        m_surface,
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1, // imageArrayLayers
        vk::ImageUsageFlagBits::eColorAttachment
    );

    QueueFamilyIndices indices = Engine::FindQueueFamilies(m_physicalDevice, m_surface);
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
        m_swapChain = m_device->createSwapchainKHRUnique(createInfo);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create swapchain.");
    }

    m_swapChainImages = m_device->getSwapchainImagesKHR(*m_swapChain);
    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}
void Swapchain::createImageViews()
{

}