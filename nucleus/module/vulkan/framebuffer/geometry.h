#ifndef NUVK_FRAMEBUFFER_GEOMETRY_H
#define NUVK_FRAMEBUFFER_GEOMETRY_H

#include <nucleus/module/vulkan/image/geometry.h>

typedef struct {
    VkFramebuffer framebuffer;
} nuvk_framebuffer_geometry_t;

nu_result_t nuvk_framebuffer_geometry_create(
    nuvk_framebuffer_geometry_t *framebuffer,
    const nuvk_context_t *context,
    const nuvk_image_geometry_t *image,
    VkRenderPass geometry_renderpass
);
nu_result_t nuvk_framebuffer_geometry_destroy(
    nuvk_framebuffer_geometry_t *framebuffer,
    const nuvk_context_t *context
);

#endif