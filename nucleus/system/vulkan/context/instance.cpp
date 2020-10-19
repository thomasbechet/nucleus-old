#include "instance.hpp"

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
        bool enableValidationLayers
    )
    {
        auto layers = Context::GetRequiredValidationLayers();
        if (enableValidationLayers && !CheckValidationLayerSupport(layers)) {
            Engine::Interrupt("Validation layers requested, but not available.");
        }

        auto appInfo = vk::ApplicationInfo(
            "Vulkan Renderer",
            VK_MAKE_VERSION(1, 0, 0),
            "Nucleus Engine",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_0
        );

        auto extensions = Context::GetRequiredExtensions();

        auto createInfo = vk::InstanceCreateInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            0, nullptr, // enabled layers
            static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
        );

        auto validationLayers = requiredValidationLayers;
        if (validationLayers.size() > 0) { // adding enabled layers
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }

        try {
            return vk::createInstanceUnique(createInfo, nullptr);
        } catch(vk::SystemError &err) {
            Logger::Fatal(Context::Section, err.what());
            Engine::Interrupt("Failed to create instance.");
        }
    }
}

struct Instance::Internal
{
    vk::UniqueInstance instance;

    Internal(
        bool enableValidationLayers
    )
    {
        instance = ::CreateInstance(enableValidationLayers);
    }
    ~Internal()
    {

    }
};

Instance::Instance(
    bool enableValidationLayers
) : internal(MakeInternalPtr<Internal>(enableValidationLayers)) {}

vk::UniqueInstance &Instance::getInstance()
{
    return internal->instance;
}