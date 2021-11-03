#ifndef NUVK_IMAGE_GEOMETRY_H
#define NUVK_IMAGE_GEOMETRY_H

#include <nucleus/module/vulkan/core/image.h>
#include <nucleus/module/vulkan/core/swapchain.h>

typedef struct {
    nuvk_image_t normal_depth;
    nuvk_image_t position_material;
} nuvk_image_geometry_t;

nu_result_t nuvk_image_geometry_create(
    nuvk_image_geometry_t *geometry,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nu_vec2u_t resolution
);
nu_result_t nuvk_image_geometry_destroy(
    nuvk_image_geometry_t *geometry,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
);

#endif