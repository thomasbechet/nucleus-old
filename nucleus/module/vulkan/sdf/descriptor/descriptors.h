#ifndef NUVK_SDF_DESCRIPTORS_H
#define NUVK_SDF_DESCRIPTORS_H

#include <nucleus/module/vulkan/sdf/buffer/buffers.h>
#include <nucleus/module/vulkan/sdf/descriptor/low_frequency.h>

typedef struct {
    VkDescriptorPool pool;
    nuvk_sdf_descriptor_low_frequency_t low_frequency;
} nuvk_sdf_descriptors_t;

nu_result_t nuvk_sdf_descriptors_initialize(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context,
    const nuvk_sdf_buffers_t *buffers
);
nu_result_t nuvk_sdf_descriptors_terminate(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context
);

#endif