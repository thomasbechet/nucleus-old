#ifndef NUVK_DESCRIPTOR_POSTPROCESS_H
#define NUVK_DESCRIPTOR_POSTPROCESS_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/image/light.h>

typedef struct {
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout layout;
    VkSampler sampler;
} nuvk_descriptor_postprocess_t;

nu_result_t nuvk_descriptor_postprocess_create(
    nuvk_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_image_light_t *image,
    VkDescriptorPool pool
);
nu_result_t nuvk_descriptor_postprocess_destroy(
    nuvk_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context
);
nu_result_t nuvk_descriptor_postprocess_update_images(
    nuvk_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_image_light_t *image
);

#endif