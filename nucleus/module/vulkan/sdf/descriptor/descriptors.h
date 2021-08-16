#ifndef NUVK_SDF_DESCRIPTORS_H
#define NUVK_SDF_DESCRIPTORS_H

#include <nucleus/module/vulkan/sdf/buffer/buffers.h>
#include <nucleus/module/vulkan/sdf/image/images.h>
#include <nucleus/module/vulkan/sdf/descriptor/low_frequency.h>
#include <nucleus/module/vulkan/sdf/descriptor/postprocess.h>

typedef struct {
    VkDescriptorPool pool;
    nuvk_sdf_descriptor_low_frequency_t low_frequency;
    nuvk_sdf_descriptor_postprocess_t postprocess;
} nuvk_sdf_descriptors_t;

nu_result_t nuvk_sdf_descriptors_initialize(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context,
    const nuvk_sdf_buffers_t *buffers,
    const nuvk_sdf_images_t *images
);
nu_result_t nuvk_sdf_descriptors_terminate(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_descriptor_update_swapchain(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context,
    const nuvk_sdf_images_t *images
);

#endif