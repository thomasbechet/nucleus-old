#ifndef NUVK_SDF_DESCRIPTOR_LIGHT_H
#define NUVK_SDF_DESCRIPTOR_LIGHT_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/sdf/image/geometry.h>
#include <nucleus/module/vulkan/sdf/image/light.h>

typedef struct {
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout layout;
} nuvk_sdf_descriptor_light_t;

nu_result_t nuvk_sdf_descriptor_light_create(
    nuvk_sdf_descriptor_light_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *geometry_image,
    const nuvk_sdf_image_light_t *light_image,
    VkDescriptorPool pool
);
nu_result_t nuvk_sdf_descriptor_light_destroy(
    nuvk_sdf_descriptor_light_t *descriptor,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_descriptor_light_update_images(
    nuvk_sdf_descriptor_light_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *geometry_image,
    const nuvk_sdf_image_light_t *light_image
);

#endif