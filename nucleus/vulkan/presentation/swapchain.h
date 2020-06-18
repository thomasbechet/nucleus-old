#ifndef NU_SWAPCHAIN_H
#define NU_SWAPCHAIN_H

#include "../renderer.h"

nu_result_t nuvk_swapchain_create(void);
nu_result_t nuvk_swapchain_destroy(void);

bool nuvk_swapchain_is_physical_device_suitable(VkPhysicalDevice physical_device);

VkSwapchainKHR nuvk_swapchain_get_handle(void);
VkExtent2D nuvk_swapchain_get_extent(void);
VkFormat nuvk_swapchain_get_format(void);
const VkImageView *nuvk_swapchain_get_image_views(uint32_t *count);
uint32_t nuvk_swapchain_get_next_image(void);

#endif