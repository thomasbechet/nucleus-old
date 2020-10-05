#ifndef NUVK_DEVICE_H
#define NUVK_DEVICE_H

#include "context.h"

nu_result_t nuvk_device_create(VkDevice *device, nuvk_queues_t *queues, VkSurfaceKHR surface, VkPhysicalDevice physical_device);
nu_result_t nuvk_device_destroy(VkDevice device);

#endif