#ifndef NU_SWAPCHAIN_H
#define NU_SWAPCHAIN_H

#include "context.h"

nu_result_t nuvk_swapchain_create(nuvk_swapchain_t *swapchain, VkSurfaceKHR surface, VkPhysicalDevice physical_device, VkDevice device);
nu_result_t nuvk_swapchain_destroy(nuvk_swapchain_t swapchain, VkDevice device);

bool nuvk_swapchain_is_physical_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

#endif