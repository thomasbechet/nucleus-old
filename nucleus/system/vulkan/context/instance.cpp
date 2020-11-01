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
        auto availableLayers = vk::enumerateInstanceLayerProperties();

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

    static vk::UniqueInstance CreateInstance(
        const WindowInterface &interface,
        bool enableValidationLayers
    )
    {
        auto requiredValidationLayers = Device::GetRequiredValidationLayers();
        if (enableValidationLayers && !CheckValidationLayerSupport(requiredValidationLayers)) {
            Engine::Interrupt("Validation layers requested, but not available.");
        }

        auto appInfo = vk::ApplicationInfo(
            "Vulkan Renderer",
            VK_MAKE_VERSION(1, 0, 0),
            "Nucleus Engine",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_0
        );

        auto requiredExtensions = Instance::GetRequiredExtensions(interface, enableValidationLayers);

        auto createInfo = vk::InstanceCreateInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            0, nullptr, // enabled layers
            static_cast<uint32_t>(requiredExtensions.size()), requiredExtensions.data() // enabled extensions
        );

        auto validationLayers = requiredValidationLayers;
        if (validationLayers.size() > 0) { // adding enabled layers
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }

        try {
            return vk::createInstanceUnique(createInfo, nullptr);
        } catch(vk::SystemError &err) {
            Logger::Fatal(Instance::Section, err.what());
            Engine::Interrupt("Failed to create instance.");
        }

        throw std::runtime_error("Unknown error.");
    }
}

struct Instance::Internal
{
    vk::UniqueInstance instance;

    Internal(
        const WindowInterface &interface,
        bool enableValidationLayers
    )
    {
        instance = ::CreateInstance(interface, enableValidationLayers);
    }
    ~Internal()
    {

    }
};

Instance::Instance(
    const WindowInterface &interface,
    bool enableValidationLayers
) : internal(MakeInternalPtr<Internal>(interface, enableValidationLayers)) {}

const vk::Instance &Instance::getInstance() const
{
    return *internal->instance;
}

std::vector<const char*> Instance::GetRequiredExtensions(const WindowInterface &interface, bool useValidationLayers)
{
    std::vector<const char*> extensions = interface.getRequiredInstanceExtensions();
    if (useValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}