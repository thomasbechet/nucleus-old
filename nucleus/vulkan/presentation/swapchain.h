#ifndef NU_SWAPCHAIN_H
#define NU_SWAPCHAIN_H

#include "../renderer.h"

nu_result_t nuvk_swapchain_create(void);
nu_result_t nuvk_swapchain_destroy(void);

bool nuvk_swapchain_is_physical_device_suitable(VkPhysicalDevice physical_device);

#endif