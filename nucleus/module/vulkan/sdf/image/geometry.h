#ifndef NUVK_SDF_IMAGE_GEOMETRY_H
#define NUVK_SDF_IMAGE_GEOMETRY_H

#include <nucleus/module/vulkan/core/image.h>
#include <nucleus/module/vulkan/core/swapchain.h>

typedef struct {
    nuvk_image_t image;
} nuvk_sdf_image_geometry_t;

nu_result_t nuvk_sdf_image_geometry_create(
    nuvk_sdf_image_geometry_t *geometry,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
);
nu_result_t nuvk_sdf_image_geometry_destroy(
    nuvk_sdf_image_geometry_t *geometry,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
);

#endif