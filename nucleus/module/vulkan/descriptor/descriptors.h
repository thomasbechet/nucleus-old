#ifndef NUVK_DESCRIPTORS_H
#define NUVK_DESCRIPTORS_H

#include <nucleus/module/vulkan/descriptor/pool.h>
#include <nucleus/module/vulkan/descriptor/low_frequency.h>
#include <nucleus/module/vulkan/descriptor/light.h>
#include <nucleus/module/vulkan/descriptor/postprocess.h>

typedef struct {
    VkDescriptorPool pool;
    nuvk_descriptor_low_frequency_t low_frequency;
    nuvk_descriptor_light_t light;
    nuvk_descriptor_postprocess_t postprocess;
} nuvk_descriptors_t;

#endif