#ifndef NUVK_SDF_DESCRIPTOR_POSTPROCESS_H
#define NUVK_SDF_DESCRIPTOR_POSTPROCESS_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/sdf/image/geometry.h>

typedef struct {
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout layout;
    VkSampler sampler;
} nuvk_sdf_descriptor_postprocess_t;

nu_result_t nuvk_sdf_descriptor_postprocess_create(
    nuvk_sdf_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *image,
    VkDescriptorPool pool
);
nu_result_t nuvk_sdf_descriptor_postprocess_destroy(
    nuvk_sdf_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_descriptor_postprocess_update_swapchain(
    nuvk_sdf_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *image
);

#endif