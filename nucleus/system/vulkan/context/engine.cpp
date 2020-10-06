#include "engine.hpp"

#include <nucleus/nucleus.h>
#include "../utility/logger.hpp"

#include <set>

using namespace nuvk;

Engine::Engine()
{
    initialize();
}
Engine::~Engine()
{
    terminate();
}

void Engine::render()
{
    
}

void Engine::Interrupt(std::string msg)
{
    nu_interrupt(("[NUVK] " + msg + "\n").c_str());
}
std::vector<const char*> Engine::GetRequiredExtensions(GLFWInterface &glfwInterface)
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwInterface->get_required_instance_extensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (Engine::USE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
std::vector<const char*> Engine::GetRequiredValidationLayers()
{
    return {
        "VK_LAYER_KHRONOS_validation"
    };
}
std::vector<const char*> Engine::GetRequiredDeviceExtensions()
{
    return {
        "VK_KHR_swapchain"
    };
}
bool Engine::CheckValidationLayerSupport()
{
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : Engine::GetRequiredValidationLayers()) {
        bool layerFound = false;
        for (const auto &layerProperties : availableLayers) {
            if (std::string(layerName) == std::string((const char*)layerProperties.layerName)) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}
bool Engine::CheckDeviceExtensionSupport(vk::PhysicalDevice device)
{
    std::vector<const char*> deviceExtensions = Engine::GetRequiredDeviceExtensions();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : device.enumerateDeviceExtensionProperties()) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
bool Engine::IsDeviceSuitable(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = Engine::FindQueueFamilies(device, surface);

    bool extensionsSupported = Engine::CheckDeviceExtensionSupport(device);
    
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = Engine::QuerySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    
    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}
QueueFamilyIndices Engine::FindQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    auto queueFamilies = device.getQueueFamilyProperties();
    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        if (queueFamily.queueCount > 0 && device.getSurfaceSupportKHR(i, surface)) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) break;
    
        i++;
    }
    return indices;
}
SwapChainSupportDetails Engine::QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}
vk::SurfaceFormatKHR Engine::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
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
vk::PresentModeKHR Engine::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes)
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
vk::Extent2D Engine::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        vk::Extent2D actualExtent = {static_cast<uint32_t>(Engine::WIDTH), static_cast<uint32_t>(Engine::HEIGHT)};
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

void Engine::initialize()
{
    Logger::Info("Create instance..."); createInstance();
    Logger::Info("Setup debug callback..."); setupDebugCallback();
    Logger::Info("Create surface..."); createSurface();
    Logger::Info("Pick physical device..."); pickPhysicalDevice();
    Logger::Info("Create logical device..."); createLogicalDevice();
    Logger::Info("Create swapchain..."); createSwapChain();
    Logger::Info("Create image views..."); createImageViews();
}
void Engine::terminate()
{
    m_device->destroySwapchainKHR(m_swapChain); /* destruction order */
    m_instance->destroySurfaceKHR(m_surface); /* not using vk::UniqueSurfaceKHR */
}

void Engine::createInstance()
{
    if (Engine::USE_VALIDATION_LAYERS && !Engine::CheckValidationLayerSupport()) {
        Engine::Interrupt("Validation layers requested, but not available.");
    }

    auto appInfo = vk::ApplicationInfo(
        "Vulkan Renderer",
        VK_MAKE_VERSION(1, 0, 0),
        "Nucleus Engine",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_0
    );

    auto extensions = Engine::GetRequiredExtensions(m_glfwInterface);

    auto createInfo = vk::InstanceCreateInfo(
        vk::InstanceCreateFlags(),
        &appInfo,
        0, nullptr, // enabled layers
        static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
    );

    auto validationLayers = Engine::GetRequiredValidationLayers();
    if (Engine::USE_VALIDATION_LAYERS) { // adding enabled layers
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    try {
        m_instance = vk::createInstanceUnique(createInfo, nullptr);
    } catch(vk::SystemError &err) {
        Logger::Fatal(err.what());
        Engine::Interrupt("Failed to create instance.");
    }
}
void Engine::setupDebugCallback()
{
    if (!Engine::USE_VALIDATION_LAYERS) return;
    m_debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(m_instance);
}
void Engine::createSurface()
{
    VkSurfaceKHR rawSurface;
    VkInstance instance = *m_instance;
    if (m_glfwInterface->create_window_surface(&instance, &rawSurface) != NU_SUCCESS) {
        Engine::Interrupt("Failed to create surface from GLFW.");
    }
    m_surface = rawSurface;
}
void Engine::pickPhysicalDevice()
{
    auto devices = m_instance->enumeratePhysicalDevices();
    if (devices.size() == 0) {
        Engine::Interrupt("Failed to find GPUs with Vulkan support.");
    }

    for (const auto &device : devices) {
        if (Engine::IsDeviceSuitable(device, m_surface)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (!m_physicalDevice) {
        Engine::Interrupt("Failed to find suitable GPU.");
    }
}
void Engine::createLogicalDevice()
{
    QueueFamilyIndices indices = Engine::FindQueueFamilies(m_physicalDevice, m_surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        queueCreateInfos.push_back({
            vk::DeviceQueueCreateFlags(),
            queueFamily,
            1, // queue count
            &queuePriority
        });
    }

    auto createInfo = vk::DeviceCreateInfo(
        vk::DeviceCreateFlags(),
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data()
    );

    auto deviceFeatures = vk::PhysicalDeviceFeatures();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    auto deviceExtensions = Engine::GetRequiredDeviceExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames =  deviceExtensions.data();

    auto layers = Engine::GetRequiredValidationLayers();
    if (Engine::USE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
    }

    try {
        m_device = m_physicalDevice.createDeviceUnique(createInfo);
    } catch(vk::SystemError &err) {
        Engine::Interrupt("Failed to create logical device.");
    }

    m_graphicsQueue = m_device->getQueue(indices.graphicsFamily.value(), 0);
    m_presentQueue = m_device->getQueue(indices.presentFamily.value(), 0);
}
void Engine::createSwapChain()
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
        m_swapChain = m_device->createSwapchainKHR(createInfo);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create swapchain.");
    }

    m_swapChainImages = m_device->getSwapchainImagesKHR(m_swapChain);
    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}
void Engine::createImageViews()
{
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo = {};
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = m_swapChainImageFormat;
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
            m_swapChainImageViews[i] = m_device->createImageViewUnique(createInfo);
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create image views.");
        }
    }
}