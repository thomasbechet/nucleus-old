#include "context.hpp"

#include <optional>

using namespace nuvk;

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

Context::Context(bool enableValidationLayers) : 
    Loggable("CONTEXT"),
    m_useValidationLayers(enableValidationLayers)
{
    Logger::Warning(this, "hello");

    // Create GLFW interface
    m_glfwInterface = std::make_unique<GLFWInterface>();

    // Recover required glfw interface
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = (*m_glfwInterface)->get_required_instance_extensions(&glfwExtensionCount);
    std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // Save required extensions and layers
    //m_requiredExtensions = requiredExtensions;
    //m_requiredDeviceExtensions = deviceExtensions;
    //m_requiredValidationLayers = validationLayers;

    createInstance();
    setupDebugCallback();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
}
Context::~Context()
{

}

const vk::Device &Context::getDevice() const
{
    return *m_device;
}
const vk::Queue &Context::getGraphicsQueue() const
{
    return m_graphicsQueue;
}
const vk::Queue &Context::getPresentQueue() const
{
    return m_presentQueue;
}

void Context::createInstance()
{
    if (m_useValidationLayers && !Context::CheckValidationLayerSupport()) {
        Engine::Interrupt("Validation layers requested, but not available.");
    }

    auto appInfo = vk::ApplicationInfo(
        "Vulkan Renderer",
        VK_MAKE_VERSION(1, 0, 0),
        "Nucleus Engine",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_0
    );

    auto extensions = Context::GetRequiredExtensions(m_glfwInterface);

    auto createInfo = vk::InstanceCreateInfo(
        vk::InstanceCreateFlags(),
        &appInfo,
        0, nullptr, // enabled layers
        static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
    );

    auto validationLayers = Context::GetRequiredValidationLayers();
    if (m_useValidationLayers) { // adding enabled layers
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    try {
        m_instance = vk::createInstanceUnique(createInfo, nullptr);
    } catch(vk::SystemError &err) {
        Logger::Fatal(this, err.what());
        Engine::Interrupt("Failed to create instance.");
    }
}
void Context::setupDebugCallback()
{
    if (!m_useValidationLayers) return;
    m_debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(m_instance);
}
void Context::createSurface()
{
    VkSurfaceKHR rawSurface;
    VkInstance instance = *m_instance;
    if (m_glfwInterface->create_window_surface(&instance, &rawSurface) != NU_SUCCESS) {
        Engine::Interrupt("Failed to create surface from GLFW.");
    }
    m_surface = rawSurface;
}
void Context::pickPhysicalDevice()
{
    auto devices = m_instance->enumeratePhysicalDevices();
    if (devices.size() == 0) {
        Engine::Interrupt("Failed to find GPUs with Vulkan support.");
    }

    for (const auto &device : devices) {
        if (Context::IsDeviceSuitable(device, m_surface)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (!m_physicalDevice) {
        Engine::Interrupt("Failed to find suitable GPU.");
    }
}
void Context::createLogicalDevice()
{
    QueueFamilyIndices indices = Context::FindQueueFamilies(m_physicalDevice, m_surface);

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

    auto deviceExtensions = Context::GetRequiredDeviceExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames =  deviceExtensions.data();

    auto layers = Context::GetRequiredValidationLayers();
    if (m_useValidationLayers) {
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

std::vector<const char*> GetRequiredExtensions(GLFWInterface &glfwInterface, bool useValidationLayers)
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwInterface->get_required_instance_extensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (useValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
std::vector<const char*> GetRequiredValidationLayers()
{
    return {
        "VK_LAYER_KHRONOS_validation"
    };
}
std::vector<const char*> GetRequiredDeviceExtensions()
{
    return {
        "VK_KHR_swapchain"
    };
}
bool CheckValidationLayerSupport()
{
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : Context::GetRequiredValidationLayers()) {
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
bool CheckDeviceExtensionSupport(vk::PhysicalDevice device)
{
    std::vector<const char*> deviceExtensions = Context::GetRequiredDeviceExtensions();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : device.enumerateDeviceExtensionProperties()) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
bool IsDeviceSuitable(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = Context::FindQueueFamilies(device, surface);

    bool extensionsSupported = Context::CheckDeviceExtensionSupport(device);
    
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = Context::QuerySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    
    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}
QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface)
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