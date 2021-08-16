#ifndef NUVK_SDF_DESCRIPTOR_LOW_FREQUENCY_H
#define NUVK_SDF_DESCRIPTOR_LOW_FREQUENCY_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/sdf/buffer/environment.h>
#include <nucleus/module/vulkan/sdf/buffer/instances.h>

typedef struct {
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout layout;
} nuvk_sdf_descriptor_low_frequency_t;

nu_result_t nuvk_sdf_descriptor_low_frequency_create(
    nuvk_sdf_descriptor_low_frequency_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_buffer_environment_t *environment_buffer,
    const nuvk_sdf_buffer_instances_t *instances_buffer,
    VkDescriptorPool pool
);
nu_result_t nuvk_sdf_descriptor_low_frequency_destroy(
    nuvk_sdf_descriptor_low_frequency_t *descriptor,
    const nuvk_context_t *context
);

#endif