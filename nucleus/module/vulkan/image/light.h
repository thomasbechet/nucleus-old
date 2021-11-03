#ifndef NUVK_IMAGE_LIGHT_H
#define NUVK_IMAGE_LIGHT_H

#include <nucleus/module/vulkan/core/image.h>
#include <nucleus/module/vulkan/core/swapchain.h>

typedef struct {
    nuvk_image_t image;
} nuvk_image_light_t;

nu_result_t nuvk_image_light_create(
    nuvk_image_light_t *light,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nu_vec2u_t resolution
);
nu_result_t nuvk_image_light_destroy(
    nuvk_image_light_t *light,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
);

#endif