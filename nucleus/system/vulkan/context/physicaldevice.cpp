#include "physicaldevice.hpp"

#include "../engine/engine.hpp"
#include "swapchain.hpp"
#include "device.hpp"

#include <set>

using namespace nuvk;

namespace
{
    static bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        std::vector<const char*> deviceExtensions = Device::GetRequiredDeviceExtensions();
        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

        for (const auto &extension : extensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    static bool IsSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
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

    static VkPhysicalDevice PickPhysicalDevice(
        VkInstance instance,
        VkSurfaceKHR surface
    )
    {
        VkPhysicalDevice physicalDevice;

        uint32_t physicalDeviceCount;
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());

        if (devices.size() == 0) {
            Engine::Interrupt(PhysicalDevice::Section, "Failed to find GPUs with Vulkan support.");
        }

        for (const auto &device : devices) {
            if (::IsSuitable(device, surface)) {
                physicalDevice = device;
                break;
            }
        }

        if (!physicalDevice) {
            Engine::Interrupt(PhysicalDevice::Section, "Failed to find suitable GPU.");
        }

        return physicalDevice;
    }
}

struct PhysicalDevice::Internal
{
    VkPhysicalDevice physicalDevice;

    Internal(
        const Instance &instance,
        const Surface &surface
    )
    {
        physicalDevice = ::PickPhysicalDevice(instance.getInstance(), surface.getSurface()); 
    }
    ~Internal()
    {

    }
};

PhysicalDevice::PhysicalDevice(
    const Instance &instance,
    const Surface &surface
) : internal(MakeInternalPtr<Internal>(instance, surface)) {}

VkPhysicalDevice PhysicalDevice::getPhysicalDevice() const
{
    return internal->physicalDevice;
}
uint32_t PhysicalDevice::getMemoryTypeIndex(
    uint32_t filter,
    VkMemoryPropertyFlags flags
) const
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(internal->physicalDevice, &memoryProperties);

    for (uint32_t index = 0; index < memoryProperties.memoryTypeCount; index++) {
        if ((filter & (1 << index)) && (memoryProperties.memoryTypes[index].propertyFlags & flags) == flags) {
            return index;
        }
    }

    Engine::Interrupt(PhysicalDevice::Section, "Failed to find suitable memory type.");
    return 0;
}

QueueFamilyIndices PhysicalDevice::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    QueueFamilyIndices indices;
    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 supported;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supported);
        if (queueFamily.queueCount > 0 && supported) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) break;
    
        i++;
    }
    return indices;
}