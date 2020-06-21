#ifndef NUVK_CONTEXT_H
#define NUVK_CONTEXT_H

#include "../renderer.h"

typedef struct {
    uint32_t graphics_family;
    bool has_graphics_family;
    uint32_t presentation_family;
    bool has_presentation_family;
} nuvk_queue_family_indices_t;

typedef struct {
    VkQueue graphics;
    VkQueue presentation;
} nuvk_queues_t;

typedef struct {
    VkSwapchainKHR handle;
    VkFormat format;
    VkExtent2D extent;
    VkPresentModeKHR present_mode;
    uint32_t image_count;
    VkImageView *images;
} nuvk_swapchain_t;

typedef struct {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physical_device;
    nuvk_queue_family_indices_t queue_family_indices;
    VkDevice device;
    nuvk_queues_t queues;
    nuvk_swapchain_t swapchain;
} nuvk_context_t;

nu_result_t nuvk_context_create(void);
nu_result_t nuvk_context_destroy(void);
const nuvk_context_t *nuvk_context_get(void);

#endif 