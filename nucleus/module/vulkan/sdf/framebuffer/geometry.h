#ifndef NUVK_SDF_FRAMEBUFFER_GEOMETRY_H
#define NUVK_SDF_FRAMEBUFFER_GEOMETRY_H

#include <nucleus/module/vulkan/sdf/image/geometry.h>

typedef struct {
    VkFramebuffer framebuffer;
} nuvk_sdf_framebuffer_geometry_t;

nu_result_t nuvk_sdf_framebuffer_geometry_create(
    nuvk_sdf_framebuffer_geometry_t *framebuffer,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *image,
    VkRenderPass geometry_renderpass
);
nu_result_t nuvk_sdf_framebuffer_geometry_destroy(
    nuvk_sdf_framebuffer_geometry_t *framebuffer,
    const nuvk_context_t *context
);

#endif