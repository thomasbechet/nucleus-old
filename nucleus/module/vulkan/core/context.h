#ifndef NUVK_CONTEXT_H
#define NUVK_CONTEXT_H

#include <nucleus/nucleus.h>
#include <vulkan/vulkan.h>
#include <nucleus/module/vulkan/common/logger.h>

#define NUVK_CONTEXT_ENABLE_VALIDATION_LAYER true
#define NUVK_CONTEXT_API_VERSION             VK_API_VERSION_1_2

typedef struct {
        
    VkQueue graphics_compute;
    uint32_t graphics_compute_family_index;
    uint32_t graphics_compute_index;
    
    VkQueue present;
    uint32_t present_family_index;
    uint32_t present_index;

    bool single_graphics_present_queue;
} nuvk_context_queues_t;

typedef struct {
    VkAllocationCallbacks allocator;
    VkInstance instance;
    VkDebugReportCallbackEXT debug_report_callback;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties physical_device_properties;
    VkSurfaceKHR surface;
    VkDevice device;
    nuvk_context_queues_t queues;
} nuvk_context_t;

nu_result_t nuvk_context_initialize(nuvk_context_t *context);
nu_result_t nuvk_context_terminate(nuvk_context_t *context);

#endif