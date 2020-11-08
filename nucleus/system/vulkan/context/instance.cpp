#include "instance.hpp"

#include "../utility/logger.hpp"
#include "../engine/engine.hpp"
#include "device.hpp"

using namespace nuvk;

namespace
{
    static bool CheckValidationLayerSupport(
        const std::vector<const char*> &instanceLayers
    )
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName : instanceLayers) {
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

    static VkInstance CreateInstance(
        const WindowInterface &interface,
        bool enableValidationLayers
    )
    {
        auto requiredValidationLayers = Device::GetRequiredValidationLayers();
        if (enableValidationLayers && !CheckValidationLayerSupport(requiredValidationLayers)) {
            Engine::Interrupt(Instance::Section, "Validation layers requested, but not available.");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = "Vulkan Renderer";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = "Nucleus Engine";
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion         = VK_API_VERSION_1_0;

        auto requiredExtensions = Instance::GetRequiredExtensions(interface, enableValidationLayers);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo        = &appInfo;
        createInfo.enabledLayerCount       = 0;
        createInfo.ppEnabledLayerNames     = nullptr;
        createInfo.enabledExtensionCount   = requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        auto validationLayers = requiredValidationLayers;
        if (validationLayers.size() > 0) { // adding enabled layers
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }

        VkInstance instance;
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            Engine::Interrupt(Instance::Section, "Failed to create instance.");
        }
        return instance;
    }
}

struct Instance::Internal
{
    VkInstance instance;

    Internal(
        const WindowInterface &interface,
        bool enableValidationLayers
    )
    {
        instance = ::CreateInstance(interface, enableValidationLayers);
    }
    ~Internal()
    {
        vkDestroyInstance(instance, nullptr);
    }
};

Instance::Instance(
    const WindowInterface &interface,
    bool enableValidationLayers
) : internal(MakeInternalPtr<Internal>(interface, enableValidationLayers)) {}

VkInstance Instance::getInstance() const
{
    return internal->instance;
}

std::vector<const char*> Instance::GetRequiredExtensions(const WindowInterface &interface, bool useValidationLayers)
{
    std::vector<const char*> extensions = interface.getRequiredInstanceExtensions();
    if (useValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}