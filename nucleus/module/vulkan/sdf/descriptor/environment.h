#ifndef NUVK_SDF_DESCRIPTOR_ENVIRONMENT_H
#define NUVK_SDF_DESCRIPTOR_ENVIRONMENT_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/sdf/buffer/environment.h>

typedef struct {
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout layout;
} nuvk_sdf_descriptor_environment_t;

nu_result_t nuvk_sdf_descriptor_environment_create(
    nuvk_sdf_descriptor_environment_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_buffer_environment_t *environment_buffer,
    VkDescriptorPool pool
);
nu_result_t nuvk_sdf_descriptor_environment_destroy(
    nuvk_sdf_descriptor_environment_t *descriptor,
    const nuvk_context_t *context
);

#endif