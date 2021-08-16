#ifndef NUVK_SDF_PIPELINE_GEOMETRY_H
#define NUVK_SDF_PIPELINE_GEOMETRY_H

#include <nucleus/module/vulkan/sdf/shader/geometry.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>

typedef struct {
    VkPipelineLayout layout;
    VkPipeline pipeline;
} nuvk_sdf_pipeline_geometry_t;

nu_result_t nuvk_sdf_pipeline_geometry_create(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shader_geometry_t *shader,
    const nuvk_sdf_descriptors_t *descriptors,
    VkRenderPass geometry_renderpass
);
nu_result_t nuvk_sdf_pipeline_geometry_destroy(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_pipeline_geometry_update_swapchain(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shader_geometry_t *shader,
    VkRenderPass geometry_renderpass
);

#endif