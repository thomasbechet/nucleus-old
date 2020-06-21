#include "physical_device.h"

#include "../common/logger.h"
#include "swapchain.h"
#include "instance.h"
#include "surface.h"

static bool is_queue_family_indices_suitable(nuvk_queue_family_indices_t indices)
{
    return indices.has_graphics_family && indices.has_presentation_family;
}
static uint32_t rate_physical_device(VkPhysicalDevice device)
{
    uint32_t score;
    score = 0;

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1;
    }

    score += device_properties.limits.maxImageDimension2D;

    if (!device_features.geometryShader) {
        return 0;
    }

    return score;
}
static bool check_extensions_support(VkPhysicalDevice device)
{
    uint32_t required_extension_count;
    const char **required_extensions;
    required_extensions = nuvk_physical_device_get_required_extensions(&required_extension_count);

    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);

    VkExtensionProperties *extensions;
    extensions = (VkExtensionProperties*)nu_malloc(sizeof(VkExtensionProperties) * extension_count);
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, extensions);

    bool all_extension_found = true;
    for (uint32_t i = 0; i < required_extension_count && all_extension_found; i++) {
        all_extension_found = false;
        for (uint32_t j = 0; j < extension_count; j++) {
            if (strcmp(required_extensions[i], extensions[j].extensionName) == 0) {
                all_extension_found = true;
                break;
            }
        }
    }

    nu_free(extensions);

    return all_extension_found;
}

nu_result_t nuvk_physical_device_pick(VkPhysicalDevice *physical_device, nuvk_queue_family_indices_t *queue_family_indices, VkInstance instance, VkSurfaceKHR surface)
{
    nu_result_t result;
    result = NU_SUCCESS;
    
    uint32_t device_count;
    device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);
    if (device_count == 0) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to find physical device with vulkan support.\n");
        return NU_FAILURE;
    }

    VkPhysicalDevice *physical_devices;
    physical_devices = (VkPhysicalDevice*)nu_malloc(sizeof(VkPhysicalDevice) * device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, physical_devices);

    uint32_t current_device_score;
    current_device_score = 0;
    *physical_device = VK_NULL_HANDLE;
    /* iterate over devices */
    for (uint32_t i = 0; i < device_count; i++) {
        /* check queue families support */
        nuvk_queue_family_indices_t indices;
        nuvk_physical_device_get_queue_family_indices(&indices, surface, physical_devices[i]);
        if (is_queue_family_indices_suitable(indices)) {
            /* check extension support */
            if (check_extensions_support(physical_devices[i])) {
                /* check available swapchain details */
                if (nuvk_swapchain_is_physical_device_suitable(physical_devices[i], surface)) {
                    /* rate the device */
                    uint32_t score = rate_physical_device(physical_devices[i]);
                    if (score > current_device_score) {
                        *physical_device = physical_devices[i];
                        *queue_family_indices = indices;
                        current_device_score = score;
                    }
                }
            }
        }
    }
    
    if (*physical_device == VK_NULL_HANDLE) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to find suitable physical device.\n");
        nu_free(physical_devices);
        return NU_FAILURE;
    }

    nu_free(physical_devices);

    return result;
}
nu_result_t nuvk_physical_device_get_features(VkPhysicalDeviceFeatures *features, VkPhysicalDevice device)
{
    vkGetPhysicalDeviceFeatures(device, features);
    return NU_SUCCESS;
}
nu_result_t nuvk_physical_device_get_queue_family_indices(nuvk_queue_family_indices_t *indices, VkSurfaceKHR surface, VkPhysicalDevice device)
{
    memset(indices, 0, sizeof(nuvk_queue_family_indices_t));
    
    indices->has_graphics_family = false;

    uint32_t queue_family_count;
    VkQueueFamilyProperties *queue_families;
    queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
    queue_families = (VkQueueFamilyProperties*)nu_malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (uint32_t i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices->has_graphics_family = true;
            indices->graphics_family = i;
        } else {
            VkBool32 presentation_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentation_support);
            if (presentation_support) {
                indices->has_presentation_family = true;
                indices->presentation_family = i;
            }
        }
    }

    nu_free(queue_families);

    return NU_SUCCESS;
}
const char **nuvk_physical_device_get_required_extensions(uint32_t *count)
{
    static uint32_t required_extension_count = 1;
    static const char *required_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    *count = required_extension_count;
    return required_extensions;
}