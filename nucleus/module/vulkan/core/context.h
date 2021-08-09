#ifndef NUVK_CONTEXT_H
#define NUVK_CONTEXT_H

#include <nucleus/nucleus.h>
#include <vulkan/vulkan.h>

#define NUVK_CONTEXT_ENABLE_VALIDATION_LAYER true
#define NUVK_CONTEXT_API_VERSION             VK_API_VERSION_1_2
#define NUVK_LOGGER_NAME                     "[VULKAN] "

typedef struct {
    VkAllocationCallbacks allocator;
    VkInstance instance;
    VkDebugReportCallbackEXT debug_report_callback;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties physical_device_properties;
    VkSurfaceKHR surface;
    uint32_t graphics_queue_family_index;
    uint32_t present_queue_family_index;
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
} nuvk_context_t;

nu_result_t nuvk_context_initialize(nuvk_context_t *context);
nu_result_t nuvk_context_terminate(nuvk_context_t *context);

#endif