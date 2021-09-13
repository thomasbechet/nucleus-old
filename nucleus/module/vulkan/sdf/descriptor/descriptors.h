#ifndef NUVK_SDF_DESCRIPTORS_H
#define NUVK_SDF_DESCRIPTORS_H

#include <nucleus/module/vulkan/sdf/descriptor/pool.h>
#include <nucleus/module/vulkan/sdf/descriptor/low_frequency.h>
#include <nucleus/module/vulkan/sdf/descriptor/light.h>
#include <nucleus/module/vulkan/sdf/descriptor/postprocess.h>

typedef struct {
    VkDescriptorPool pool;
    nuvk_sdf_descriptor_low_frequency_t low_frequency;
    nuvk_sdf_descriptor_light_t light;
    nuvk_sdf_descriptor_postprocess_t postprocess;
} nuvk_sdf_descriptors_t;

#endif