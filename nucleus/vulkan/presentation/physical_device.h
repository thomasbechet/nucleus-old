#ifndef NUVK_PHYSICAL_DEVICE_H
#define NUVK_PHYSICAL_DEVICE_H

#include "../renderer.h"

typedef struct {
    uint32_t graphics_family;
    bool has_graphics_family;
    uint32_t presentation_family;
    bool has_presentation_family;
} nuvk_queue_family_indices_t;

const char **nuvk_physical_device_get_required_extensions(uint32_t *count);

nu_result_t nuvk_physical_device_pick(void);
VkPhysicalDevice nuvk_physical_device_get_handle(void);
nuvk_queue_family_indices_t nuvk_physical_device_get_queue_family_indices(void);
VkPhysicalDeviceFeatures nuvk_physical_device_get_features(void);

#endif