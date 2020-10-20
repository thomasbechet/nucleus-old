#include "logicaldevice.hpp"

#include "context.hpp"
#include "engine.hpp"

#include <set>

using namespace nuvk;

namespace
{
    static vk::UniqueDevice CreateLogicalDevice(
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

        auto deviceExtensions = Context::GetRequiredDeviceExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames =  deviceExtensions.data();

        auto layers = Context::GetRequiredValidationLayers();
        if (useValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
            createInfo.ppEnabledLayerNames = layers.data();
        }

        try {
            return physicalDevice.createDeviceUnique(createInfo);
        } catch(vk::SystemError &err) {
            Engine::Interrupt("Failed to create logical device.");
        }

        throw std::runtime_error("Unknown error.");
    }
}

struct LogicalDevice::Internal
{
    vk::UniqueDevice device;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;

    Internal(
        vk::PhysicalDevice physicalDevice,
        VkSurfaceKHR &surface,
        bool useValidationLayers
    )
    {
        device = ::CreateLogicalDevice(physicalDevice, surface, useValidationLayers);
        QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(physicalDevice, surface);
        graphicsQueue = device->getQueue(indices.graphicsFamily.value(), 0);
        presentQueue = device->getQueue(indices.presentFamily.value(), 0);
    }
    ~Internal()
    {

    }
};

LogicalDevice::LogicalDevice(
    vk::PhysicalDevice physicalDevice,
    VkSurfaceKHR surface,
    bool useValidationLayers
) : internal(MakeInternalPtr<Internal>(physicalDevice, surface, useValidationLayers)) {}

vk::UniqueDevice &LogicalDevice::getLogicalDevice()
{
    return internal->device;
}
vk::Queue &LogicalDevice::getGraphicsQueue()
{
    return internal->graphicsQueue;
}
vk::Queue &LogicalDevice::getPresentQueue()
{
    return internal->presentQueue;
}