#include "debugutilsmessenger.hpp"

#include "../engine/engine.hpp"
#include "../utility/logger.hpp"

using namespace nuvk;

namespace
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
        switch (messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            Logger::Info(DebugUtilsMessenger::Section, std::string(pCallbackData->pMessage));
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            Logger::Info(DebugUtilsMessenger::Section, std::string(pCallbackData->pMessage));
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            Logger::Warning(DebugUtilsMessenger::Section, std::string(pCallbackData->pMessage));
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            Logger::Fatal(DebugUtilsMessenger::Section, std::string(pCallbackData->pMessage));
            break;
        default:
            Logger::Info(DebugUtilsMessenger::Section, std::string(pCallbackData->pMessage));
            break;
        }

        return VK_FALSE;
    }

    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) 
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) 
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, callback, pAllocator);
        }
    }
}

struct DebugUtilsMessenger::Internal
{
    VkDebugUtilsMessengerEXT callback;
    const Instance &instance;

    Internal(
        const Instance &instance
    ) : instance(instance)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = ::DebugCallback;
        createInfo.pUserData       = nullptr;

        if (::CreateDebugUtilsMessengerEXT(instance.getInstance(), &createInfo, nullptr, &callback) != VK_SUCCESS) {
            Engine::Interrupt(DebugUtilsMessenger::Section, "Failed to setup debug callback.");
        }
    }
    ~Internal()
    {
        ::DestroyDebugUtilsMessengerEXT(instance.getInstance(), callback, nullptr);
    }
};

DebugUtilsMessenger::DebugUtilsMessenger(
    const Instance &instance
) : internal(MakeInternalPtr<Internal>(instance)) {}