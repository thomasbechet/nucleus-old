#include "physical_device.h"

#include "../common/logger.h"
#include "swapchain.h"
#include "instance.h"
#include "surface.h"

typedef struct {
    VkPhysicalDevice physical_device;
    nuvk_queue_family_indices_t indices;
    VkPhysicalDeviceFeatures features;
} nuvk_physical_device_data_t;

static nuvk_physical_device_data_t _data;

const char **nuvk_physical_device_get_required_extensions(uint32_t *count)
{
    static uint32_t required_extension_count = 1;
    static const char *required_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    *count = required_extension_count;
    return required_extensions;
}

static nuvk_queue_family_indices_t find_queue_family_indices(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    nuvk_queue_family_indices_t indices;
    memset(&indices, 0, sizeof(nuvk_queue_family_indices_t));

    indices.has_graphics_family = false;

    uint32_t queue_family_count;
    VkQueueFamilyProperties *queue_families;
    queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
    queue_families = (VkQueueFamilyProperties*)nu_malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (uint32_t i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.has_graphics_family = true;
            indices.graphics_family = i;
        } else {
            VkBool32 presentation_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentation_support);
            if (presentation_support) {
                indices.has_presentation_family = true;
                indices.presentation_family = i;
            }
        }
    }

    nu_free(queue_families);

    return indices;
}
static bool is_queue_family_indices_suitable(const nuvk_queue_family_indices_t *indices)
{
    return indices->has_graphics_family && indices->has_presentation_family;
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

nu_result_t nuvk_physical_device_pick(void)
{
    nu_result_t result;
    result = NU_SUCCESS;
    
    uint32_t device_count;
    device_count = 0;
    vkEnumeratePhysicalDevices(nuvk_instance_get_handle(), &device_count, NULL);
    if (device_count == 0) {
        nu_warning(NUVK_LOGGER_VULKAN"Failed to find physical device with vulkan support.\n");
        return NU_FAILURE;
    }

    VkPhysicalDevice *physical_devices;
    physical_devices = (VkPhysicalDevice*)nu_malloc(sizeof(VkPhysicalDevice) * device_count);
    vkEnumeratePhysicalDevices(nuvk_instance_get_handle(), &device_count, physical_devices);

    uint32_t current_device_score;
    current_device_score = 0;
    _data.physical_device = VK_NULL_HANDLE;
    /* iterate over devices */
    for (uint32_t i = 0; i < device_count; i++) {
        /* check queue families support */
        nuvk_queue_family_indices_t indices;
        indices = find_queue_family_indices(physical_devices[i], nuvk_surface_get_handle());
        if (is_queue_family_indices_suitable(&indices)) {
            /* check extension support */
            if (check_extensions_support(physical_devices[i])) {
                /* check available swapchain details */
                if (nuvk_swapchain_is_physical_device_suitable(physical_devices[i])) {
                    /* rate the device */
                    uint32_t score = rate_physical_device(physical_devices[i]);
                    if (score > current_device_score) {
                        _data.physical_device = physical_devices[i];
                        _data.indices = indices;
                        vkGetPhysicalDeviceFeatures(physical_devices[i], &_data.features);
                        current_device_score = score;
                    }
                }
            }
        }
    }
    
    if (_data.physical_device == VK_NULL_HANDLE) {
        nu_warning(NUVK_LOGGER_VULKAN"Failed to find suitable physical device.\n");
        nu_free(physical_devices);
        return NU_FAILURE;
    }

    nu_free(physical_devices);

    return result;
}
VkPhysicalDevice nuvk_physical_device_get_handle(void)
{
    return _data.physical_device;
}
nuvk_queue_family_indices_t nuvk_physical_device_get_queue_family_indices(void)
{
    return _data.indices;
}
VkPhysicalDeviceFeatures nuvk_physical_device_get_features(void)
{
    return _data.features;
}