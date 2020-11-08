#include "device.hpp"

#include "../engine/engine.hpp"

#include <set>

using namespace nuvk;

namespace
{
    static VkDevice CreateDevice(
        VkPhysicalDevice physicalDevice,
        VkSurfaceKHR surface,
        bool useValidationLayers
    )
    {
        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(physicalDevice, surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;

        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo info{};
            info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            info.queueFamilyIndex = queueFamily;
            info.queueCount       = 1;
            info.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(info);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pQueueCreateInfos    = queueCreateInfos.data();

        VkPhysicalDeviceFeatures deviceFeatures{};
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;

        auto deviceExtensions = Device::GetRequiredDeviceExtensions();
        createInfo.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        auto layers = Device::GetRequiredValidationLayers();
        if (useValidationLayers) {
            createInfo.enabledLayerCount   = static_cast<uint32_t>(layers.size());
            createInfo.ppEnabledLayerNames = layers.data();
        }

        VkDevice device;
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            Engine::Interrupt(Device::Section, "Failed to create device.");
        }
        return device;
    }
}

struct Device::Internal
{
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    uint32_t graphicsQueueIndex;
    uint32_t presentQueueIndex;

    Internal(
        const PhysicalDevice &physicalDevice,
        const Surface &surface,
        bool useValidationLayers
    )
    {
        device = ::CreateDevice(
            physicalDevice.getPhysicalDevice(), 
            surface.getSurface(), 
            useValidationLayers
        );
        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(
            physicalDevice.getPhysicalDevice(), 
            surface.getSurface()
        );
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
        graphicsQueueIndex = indices.graphicsFamily.value();
        presentQueueIndex = indices.presentFamily.value();
    }
    ~Internal()
    {
        vkDestroyDevice(device, nullptr);
    }
};

Device::Device(
    const PhysicalDevice &physicalDevice,
    const Surface &surface,
    bool useValidationLayers
) : internal(MakeInternalPtr<Internal>(physicalDevice, surface, useValidationLayers)) {}

VkDevice Device::getDevice() const
{
    return internal->device;
}
VkQueue Device::getGraphicsQueue() const
{
    return internal->graphicsQueue;
}
uint32_t Device::getGraphicsQueueIndex() const
{
    return internal->graphicsQueueIndex;
}
VkQueue Device::getPresentQueue() const
{
    return internal->presentQueue;
}
uint32_t Device::getPresentQueueIndex() const
{
    return internal->presentQueueIndex;
}

std::vector<const char*> Device::GetRequiredValidationLayers()
{
    return {
        "VK_LAYER_KHRONOS_validation"
    };
}
std::vector<const char*> Device::GetRequiredDeviceExtensions()
{
    return {
        "VK_KHR_swapchain"
    };
}