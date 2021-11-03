#ifndef NUVK_RENDERPASS_GEOMETRY_H
#define NUVK_RENDERPASS_GEOMETRY_H

#include <nucleus/module/vulkan/image/geometry.h>

nu_result_t nuvk_renderpass_geometry_create(
    VkRenderPass *renderpass,
    const nuvk_context_t *context,
    const nuvk_image_geometry_t *image
);

#endif