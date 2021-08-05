#ifndef NUVK_SWAPCHAIN_H
#define NUVK_SWAPCHAIN_H

#include <nucleus/module/vulkan/core/context.h>

typedef struct {
    VkSwapchainKHR swapchain;
    uint32_t image_count;
    VkImage *images;
    VkImageView *image_views;
    VkFormat surface_color_format;
    VkColorSpaceKHR surface_color_space;
    VkExtent2D extent; 
} nuvk_swapchain_t;

nu_result_t nuvk_swapchain_initialize(nuvk_swapchain_t *swapchain, const nuvk_context_t *context, uint32_t width, uint32_t height);
nu_result_t nuvk_swapchain_terminate(nuvk_swapchain_t *swapchain, const nuvk_context_t *context);
nu_result_t nuvk_swapchain_recreate(nuvk_swapchain_t *swapchain, const nuvk_context_t *context, uint32_t width, uint32_t height);

#endif