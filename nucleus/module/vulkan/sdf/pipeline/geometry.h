#ifndef NUVK_SDF_PIPELINE_GEOMETRY_H
#define NUVK_SDF_PIPELINE_GEOMETRY_H

#include <nucleus/module/vulkan/sdf/shader/geometry.h>

nu_result_t nuvk_sdf_pipeline_geometry_create(
    VkPipeline *pipeline,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shader_geometry_t *shader,
    VkRenderPass geometry_renderpass
);

#endif