#include "device.hpp"

#include "../engine/engine.hpp"

#include <set>

using namespace nuvk;

namespace
{
    static vk::UniqueDevice CreateDevice(
        vk::PhysicalDevice physicalDevice,
        VkSurfaceKHR surface,
        bool useValidationLayers
    )
    {
        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(physicalDevice, surface);

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

        auto deviceExtensions = Device::GetRequiredDeviceExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames =  deviceExtensions.data();

        auto layers = Device::GetRequiredValidationLayers();
        if (useValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
            createInfo.ppEnabledLayerNames = layers.data();
        }

        try {
            return physicalDevice.createDeviceUnique(createInfo);
        } catch(vk::SystemError &err) {
            Engine::Interrupt("Failed to create device.");
        }

        throw std::runtime_error("Unknown error.");
    }
}

struct Device::Internal
{
    vk::UniqueDevice device;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
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
        graphicsQueue = device->getQueue(indices.graphicsFamily.value(), 0);
        presentQueue = device->getQueue(indices.presentFamily.value(), 0);
        graphicsQueueIndex = indices.graphicsFamily.value();
        presentQueueIndex = indices.presentFamily.value();
    }
    ~Internal()
    {

    }
};

Device::Device(
    const PhysicalDevice &physicalDevice,
    const Surface &surface,
    bool useValidationLayers
) : internal(MakeInternalPtr<Internal>(physicalDevice, surface, useValidationLayers)) {}

const vk::Device &Device::getDevice() const
{
    return *internal->device;
}
const vk::Queue &Device::getGraphicsQueue() const
{
    return internal->graphicsQueue;
}
uint32_t Device::getGraphicsQueueIndex() const
{
    return internal->graphicsQueueIndex;
}
const vk::Queue &Device::getPresentQueue() const
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