#include "device.h"

#include "../common/logger.h"
#include "physical_device.h"

nu_result_t nuvk_device_create(VkDevice *device, nuvk_queues_t *queues, VkSurfaceKHR surface, VkPhysicalDevice physical_device)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* get queue family indices */
    nuvk_queue_family_indices_t indices;
    nuvk_physical_device_get_queue_family_indices(&indices, surface, physical_device);

    /* queues info */
    float queue_priority = 1.0f;
    uint32_t queue_create_info_count = 2;
    VkDeviceQueueCreateInfo *queue_create_infos;
    queue_create_infos = (VkDeviceQueueCreateInfo*)nu_malloc(sizeof(VkDeviceQueueCreateInfo) * queue_create_info_count);
    memset(queue_create_infos, 0, sizeof(VkDeviceQueueCreateInfo) * queue_create_info_count);

    /* graphics queue */
    queue_create_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[0].queueFamilyIndex = indices.graphics_family;
    queue_create_infos[0].queueCount = 1;
    queue_create_infos[0].pQueuePriorities = &queue_priority;

    /* presentation queue */
    queue_create_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[1].queueFamilyIndex = indices.presentation_family;
    queue_create_infos[1].queueCount = 1;
    queue_create_infos[1].pQueuePriorities = &queue_priority;

    /* create logical device */
    uint32_t required_extension_count;
    const char **required_extensions;
    required_extensions = nuvk_physical_device_get_required_extensions(&required_extension_count);

    VkPhysicalDeviceFeatures features;
    nuvk_physical_device_get_features(&features, physical_device);

    VkDeviceCreateInfo device_create_info;
    memset(&device_create_info, 0, sizeof(VkDeviceCreateInfo));
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = queue_create_infos;
    device_create_info.queueCreateInfoCount = queue_create_info_count;
    device_create_info.pEnabledFeatures = &features;
    device_create_info.enabledExtensionCount = required_extension_count;
    device_create_info.ppEnabledExtensionNames = required_extensions;

    if (vkCreateDevice(physical_device, &device_create_info, NULL, device) != VK_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create logical device.\n");
        nu_free(queue_create_infos);
        return NU_FAILURE;
    }

    nu_free(queue_create_infos);

    /* recover queues */
    vkGetDeviceQueue(*device, indices.graphics_family, 0, &queues->graphics);
    vkGetDeviceQueue(*device, indices.presentation_family, 0, &queues->presentation);

    return result;
}
nu_result_t nuvk_device_destroy(VkDevice device)
{
    vkDestroyDevice(device, NULL);

    return NU_SUCCESS;
}