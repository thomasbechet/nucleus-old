#include "context.hpp"

#include "../engine/engine.hpp"
#include "../utility/glfwinterface.hpp"
#include "../utility/debugutilsmessenger.hpp"
#include "../utility/logger.hpp"
#include "instance.hpp"
#include "physicaldevice.hpp"

#include <optional>
#include <set>

using namespace nuvk;

namespace
{
    
}

struct Context::Internal
{
    std::unique_ptr<GLFWInterface> glfwInterface;
    std::unique_ptr<Instance> instance;
    std::unique_ptr<DebugUtilsMessenger> debugUtilsMessenger;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physicalDevice;
    std::unique_ptr<Device> device;
    std::unique_ptr<Swapchain> swapchain;

    Internal()
    {
        const bool enableValidationLayers = true;

        // get glfw interface
        Logger::Info(Context::Section, "Getting GLFW interface...");
        glfwInterface = std::make_unique<GLFWInterface>();
        
        // create instance
        Logger::Info(Context::Section, "Creating instance...");
        instance = std::make_unique<Instance>(*glfwInterface, enableValidationLayers);
        
        // create debug utils messenger
        Logger::Info(Context::Section, "Creating debug utils messenger...");
        debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(instance->getInstance());
        
        // create surface
        Logger::Info(Context::Section, "Creating surface...");
        surface = std::make_unique<Surface>(
            instance->getInstance(),
            *glfwInterface
        );

        // create physical surface
        Logger::Info(Context::Section, "Picking physical device...");
        physicalDevice = std::make_unique<PhysicalDevice>(
            instance->getInstance(),
            surface->getSurface()  
        );

        // create device
        Logger::Info(Context::Section, "Creating device...");
        device = std::make_unique<Device>(
            physicalDevice->getPhysicalDevice(),
            surface->getSurface(),
            enableValidationLayers
        );

        // create swapchain
        Logger::Info(Context::Section, "Creating swapchain...");
        swapchain = std::make_unique<Swapchain>(
            device->getDevice(),
            physicalDevice->getPhysicalDevice(),
            surface->getSurface(),
            900, 450
        );
    }
    ~Internal()
    {
        device->getDevice().waitIdle();

        swapchain.reset();
        device.reset();
        physicalDevice.reset();
        surface.reset();
        debugUtilsMessenger.reset();
        instance.reset();
        glfwInterface.reset();
    }
};

Context::Context() : internal(MakeInternalPtr<Internal>()) {}

Device &Context::getDevice()
{
    return *internal->device;
}
Surface &Context::getSurface()
{
    return *internal->surface;
}
Swapchain &Context::getSwapchain()
{
    return *internal->swapchain;
}

std::vector<const char*> Context::GetRequiredExtensions(GLFWInterface &glfwInterface, bool useValidationLayers)
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
std::vector<const char*> Context::GetRequiredValidationLayers()
{
    return {
        "VK_LAYER_KHRONOS_validation"
    };
}
std::vector<const char*> Context::GetRequiredDeviceExtensions()
{
    return {
        "VK_KHR_swapchain"
    };
}

// static bool CheckDeviceExtensionSupport(vk::PhysicalDevice device)
// {
//     std::vector<const char*> deviceExtensions = Context::GetRequiredDeviceExtensions();
//     std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

//     for (const auto &extension : device.enumerateDeviceExtensionProperties()) {
//         requiredExtensions.erase(extension.extensionName);
//     }

//     return requiredExtensions.empty();
// }

// Context::Context(bool enableValidationLayers) : 
//     Loggable("CONTEXT"),
//     m_useValidationLayers(enableValidationLayers)
// {
//     // Create GLFW interface
//     m_glfwInterface = std::make_unique<GLFWInterface>();


//     // Initialize context
//     Logger::Info(this, "Creating instance..."); createInstance();
//     Logger::Info(this, "Setup debug callback..."); setupDebugCallback();
//     Logger::Info(this, "Creating surface..."); createSurface();
//     Logger::Info(this, "Pick physical device..."); pickPhysicalDevice();
//     Logger::Info(this, "Creating logicial device..."); createLogicalDevice();
// }
// Context::~Context()
// {
//     m_device->waitIdle();

//     //m_swapChain.reset(); /* destruction order */
//     m_instance->destroySurfaceKHR(m_surface); /* not using vk::UniqueSurfaceKHR */
// }

// const vk::Device &Context::getDevice() const
// {
//     return *m_device;
// }
// const vk::Queue &Context::getGraphicsQueue() const
// {
//     return m_graphicsQueue;
// }
// const vk::Queue &Context::getPresentQueue() const
// {
//     return m_presentQueue;
// }

// void Context::createInstance()
// {
//     if (m_useValidationLayers && !CheckValidationLayerSupport()) {
//         Engine::Interrupt("Validation layers requested, but not available.");
//     }

//     auto appInfo = vk::ApplicationInfo(
//         "Vulkan Renderer",
//         VK_MAKE_VERSION(1, 0, 0),
//         "Nucleus Engine",
//         VK_MAKE_VERSION(1, 0, 0),
//         VK_API_VERSION_1_0
//     );

//     auto extensions = GetRequiredExtensions(*m_glfwInterface, m_useValidationLayers);

//     auto createInfo = vk::InstanceCreateInfo(
//         vk::InstanceCreateFlags(),
//         &appInfo,
//         0, nullptr, // enabled layers
//         static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
//     );

//     auto validationLayers = GetRequiredValidationLayers();
//     if (m_useValidationLayers) { // adding enabled layers
//         createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//         createInfo.ppEnabledLayerNames = validationLayers.data();
//     }

//     try {
//         m_instance = vk::createInstanceUnique(createInfo, nullptr);
//     } catch(vk::SystemError &err) {
//         Logger::Fatal(this, err.what());
//         Engine::Interrupt("Failed to create instance.");
//     }
// }
// void Context::setupDebugCallback()
// {
//     if (!m_useValidationLayers) return;
//     m_debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(m_instance);
// }
// void Context::createSurface()
// {
//     VkSurfaceKHR rawSurface;
//     VkInstance instance = *m_instance;
//     if ((*m_glfwInterface)->create_window_surface(&instance, &rawSurface) != NU_SUCCESS) {
//         Engine::Interrupt("Failed to create surface from GLFW.");
//     }
//     m_surface = rawSurface;
// }
// void Context::pickPhysicalDevice()
// {
//     auto devices = m_instance->enumeratePhysicalDevices();
//     if (devices.size() == 0) {
//         Engine::Interrupt("Failed to find GPUs with Vulkan support.");
//     }

//     for (const auto &device : devices) {
//         if (IsDeviceSuitable(device, m_surface)) {
//             m_physicalDevice = device;
//             break;
//         }
//     }

//     if (!m_physicalDevice) {
//         Engine::Interrupt("Failed to find suitable GPU.");
//     }
// }
// void Context::createLogicalDevice()
// {
//     QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice, m_surface);

//     std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
//     std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

//     float queuePriority = 1.0f;

//     for (uint32_t queueFamily : uniqueQueueFamilies) {
//         queueCreateInfos.push_back({
//             vk::DeviceQueueCreateFlags(),
//             queueFamily,
//             1, // queue count
//             &queuePriority
//         });
//     }

//     auto createInfo = vk::DeviceCreateInfo(
//         vk::DeviceCreateFlags(),
//         static_cast<uint32_t>(queueCreateInfos.size()),
//         queueCreateInfos.data()
//     );

//     auto deviceFeatures = vk::PhysicalDeviceFeatures();
//     createInfo.pEnabledFeatures = &deviceFeatures;
//     createInfo.enabledExtensionCount = 0;

//     auto deviceExtensions = Context::GetRequiredDeviceExtensions();
//     createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
//     createInfo.ppEnabledExtensionNames =  deviceExtensions.data();

//     auto layers = Context::GetRequiredValidationLayers();
//     if (m_useValidationLayers) {
//         createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
//         createInfo.ppEnabledLayerNames = layers.data();
//     }

//     try {
//         m_device = m_physicalDevice.createDeviceUnique(createInfo);
//     } catch(vk::SystemError &err) {
//         Engine::Interrupt("Failed to create logical device.");
//     }

//     m_graphicsQueue = m_device->getQueue(indices.graphicsFamily.value(), 0);
//     m_presentQueue = m_device->getQueue(indices.presentFamily.value(), 0);
// }