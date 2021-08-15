#ifndef NUVK_SDF_RENDERPASS_GEOMETRY_H
#define NUVK_SDF_RENDERPASS_GEOMETRY_H

#include <nucleus/module/vulkan/sdf/image/geometry.h>

nu_result_t nuvk_sdf_renderpass_geometry_create(
    VkRenderPass *renderpass,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *image
);

#endif