#ifndef NUVK_DESCRIPTOR_LIGHT_H
#define NUVK_DESCRIPTOR_LIGHT_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/image/geometry.h>
#include <nucleus/module/vulkan/image/light.h>

typedef struct {
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout layout;
} nuvk_descriptor_light_t;

nu_result_t nuvk_descriptor_light_create(
    nuvk_descriptor_light_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_image_geometry_t *geometry_image,
    const nuvk_image_light_t *light_image,
    VkDescriptorPool pool
);
nu_result_t nuvk_descriptor_light_destroy(
    nuvk_descriptor_light_t *descriptor,
    const nuvk_context_t *context
);
nu_result_t nuvk_descriptor_light_update_images(
    nuvk_descriptor_light_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_image_geometry_t *geometry_image,
    const nuvk_image_light_t *light_image
);

#endif