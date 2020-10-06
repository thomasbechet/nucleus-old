#ifndef NUVK_PHYSICAL_DEVICE_H
#define NUVK_PHYSICAL_DEVICE_H

#include "context.h"

nu_result_t nuvk_physical_device_pick(VkPhysicalDevice *physical_device, nuvk_queue_family_indices_t *indices, VkInstance instance, VkSurfaceKHR surface);
nu_result_t nuvk_physical_device_get_features(VkPhysicalDeviceFeatures *features, VkPhysicalDevice device);
nu_result_t nuvk_physical_device_get_queue_family_indices(nuvk_queue_family_indices_t *indices, VkSurfaceKHR surface, VkPhysicalDevice device);
const char **nuvk_physical_device_get_required_extensions(uint32_t *count);

#endif