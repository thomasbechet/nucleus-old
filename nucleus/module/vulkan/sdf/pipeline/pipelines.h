#ifndef NUVK_SDF_PIPELINES_H
#define NUVK_SDF_PIPELINES_H

#include <nucleus/module/vulkan/sdf/pipeline/geometry.h>
#include <nucleus/module/vulkan/sdf/pipeline/postprocess.h>
#include <nucleus/module/vulkan/sdf/shader/shaders.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>
#include <nucleus/module/vulkan/sdf/renderpass/renderpasses.h>

typedef struct {
    nuvk_sdf_pipeline_geometry_t geometry;
    nuvk_sdf_pipeline_postprocess_t postprocess;
} nuvk_sdf_pipelines_t;

void nuvk_sdf_graphics_pipeline_default_info(VkGraphicsPipelineCreateInfo *info);

nu_result_t nuvk_sdf_pipelines_initialize(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shaders_t *shaders,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_renderpasses_t *renderpasses
);
nu_result_t nuvk_sdf_pipelines_terminate(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_pipelines_update_swapchain(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shaders_t *shaders,
    const nuvk_sdf_renderpasses_t *renderpasses
);

#endif