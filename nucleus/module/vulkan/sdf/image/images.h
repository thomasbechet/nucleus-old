#ifndef NUVK_SDF_IMAGES_H
#define NUVK_SDF_IMAGES_H

#include <nucleus/module/vulkan/sdf/image/geometry.h>
#include <nucleus/module/vulkan/sdf/image/light.h>

typedef struct {
    nuvk_sdf_image_geometry_t geometry;
    nuvk_sdf_image_light_t light;
} nuvk_sdf_images_t;

nu_result_t nuvk_sdf_images_initialize(
    nuvk_sdf_images_t *images,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
);
nu_result_t nuvk_sdf_images_terminate(
    nuvk_sdf_images_t *images,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
);
nu_result_t nuvk_sdf_images_update_swapchain(
    nuvk_sdf_images_t *images,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
);

#endif