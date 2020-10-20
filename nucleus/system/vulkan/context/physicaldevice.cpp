#include "physicaldevice.hpp"

#include "engine.hpp"
#include "swapchain.hpp"

#include <set>

using namespace nuvk;

namespace
{
    static bool CheckDeviceExtensionSupport(vk::PhysicalDevice device)
    {
        std::vector<const char*> deviceExtensions = Context::GetRequiredDeviceExtensions();
        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto &extension : device.enumerateDeviceExtensionProperties()) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    static bool IsSuitable(vk::PhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(device, surface);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);
        
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = Swapchain::QuerySwapChainSupport(device, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }
        
        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    static vk::PhysicalDevice PickPhysicalDevice(
        vk::UniqueInstance &instance,
        vk::SurfaceKHR surface
    )
    {
        vk::PhysicalDevice physicalDevice;

        auto devices = instance->enumeratePhysicalDevices();
        if (devices.size() == 0) {
            Engine::Interrupt("Failed to find GPUs with Vulkan support.");
        }

        for (const auto &device : devices) {
            if (::IsSuitable(device, surface)) {
                physicalDevice = device;
                break;
            }
        }

        if (!physicalDevice) {
            Engine::Interrupt("Failed to find suitable GPU.");
        }

        return physicalDevice;
    }
}

struct PhysicalDevice::Internal
{
    vk::PhysicalDevice physicalDevice;

    Internal(
        vk::UniqueInstance &instance,
        vk::SurfaceKHR surface
    )
    {
        physicalDevice = ::PickPhysicalDevice(instance, surface); 
    }
    ~Internal()
    {

    }
};

PhysicalDevice::PhysicalDevice(
    vk::UniqueInstance &instance,
    vk::SurfaceKHR surface
) : internal(MakeInternalPtr<Internal>(instance, surface)) {}

vk::PhysicalDevice PhysicalDevice::getPhysicalDevice()
{
    return internal->physicalDevice;
}

QueueFamilyIndices PhysicalDevice::FindQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface)
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