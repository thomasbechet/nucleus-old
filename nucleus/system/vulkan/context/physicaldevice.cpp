#include "physicaldevice.hpp"

using namespace nuvk;

namespace
{
    static vk::PhysicalDevice PickPhysicalDevice(
        vk::UniqueInstance &instance,
        vk::SurfaceKHR 
    )
    {
        auto devices = instance->enumeratePhysicalDevices();
        if (devices.size() == 0) {
            Engine::Interrupt("Failed to find GPUs with Vulkan support.");
        }

        for (const auto &device : devices) {
            if (IsDeviceSuitable(device, m_surface)) {
                m_physicalDevice = device;
                break;
            }
        }

        if (!m_physicalDevice) {
            Engine::Interrupt("Failed to find suitable GPU.");
        }
    }
}

struct PhysicalDevice::Internal
{
    vk::PhysicalDevice physicalDevice;

    Internal()
    {
        
    }
    ~Internal()
    {

    }
};

PhysicalDevice::PhysicalDevice() : internal(MakeInternalPtr<Internal>()) {}

PhysicalDevice::IsSuitable(vk::PhysicalDevice &device, vk::SurfaceKHR &surface)
{
    QueueFamilyIndices indices = FindQueueFamilies(device, surface);

    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = Swapchain::QuerySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    
    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}