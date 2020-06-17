#include "debug_messenger.h"

#include "../common/logger.h"
#include "instance.h"

typedef struct {
    VkDebugUtilsMessengerEXT debug_messenger;
} nuvk_debug_messenger_data_t;

static nuvk_debug_messenger_data_t _data;

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_message_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
)
{
    nu_info("%s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

static VkResult create_debug_utils_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void destroy_debug_utils_messenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL) {
        func(instance, debugMessenger, pAllocator);
    }
}

nu_result_t nuvk_debug_messenger_create(void)
{
    VkDebugUtilsMessengerCreateInfoEXT create_info;
    memset(&create_info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debug_message_callback;

    if (create_debug_utils_messenger(nuvk_instance_get_handle(), &create_info, NULL, &_data.debug_messenger) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to create debug utils messsenger.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_debug_messenger_destroy(void)
{
    if (_data.debug_messenger != NULL) {
        destroy_debug_utils_messenger(nuvk_instance_get_handle(), _data.debug_messenger, NULL);
    }
    return NU_SUCCESS;
}