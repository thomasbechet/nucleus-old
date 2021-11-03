#ifndef NUVK_DESCRIPTOR_LOW_FREQUENCY_H
#define NUVK_DESCRIPTOR_LOW_FREQUENCY_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/buffer/environment.h>
#include <nucleus/module/vulkan/buffer/instances.h>
#include <nucleus/module/vulkan/buffer/materials.h>

typedef struct {
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout layout;
} nuvk_descriptor_low_frequency_t;

nu_result_t nuvk_descriptor_low_frequency_create(
    nuvk_descriptor_low_frequency_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_buffer_environment_t *environment_buffer,
    const nuvk_buffer_instances_t *instances_buffer,
    const nuvk_buffer_materials_t *materials_buffer,
    VkDescriptorPool pool
);
nu_result_t nuvk_descriptor_low_frequency_destroy(
    nuvk_descriptor_low_frequency_t *descriptor,
    const nuvk_context_t *context
);

#endif