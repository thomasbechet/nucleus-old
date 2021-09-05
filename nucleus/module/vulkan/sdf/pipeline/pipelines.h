#ifndef NUVK_SDF_PIPELINES_H
#define NUVK_SDF_PIPELINES_H

#include <nucleus/module/vulkan/sdf/renderpass/renderpasses.h>
#include <nucleus/module/vulkan/sdf/pipeline/sources.h>
#include <nucleus/module/vulkan/sdf/pipeline/geometry.h>
#include <nucleus/module/vulkan/sdf/pipeline/postprocess.h>

typedef struct {
    nu_string_t sources[NUVK_SDF_PIPELINE_SOURCE_COUNT];
    nuvk_sdf_pipeline_geometry_t geometry;
    nuvk_sdf_pipeline_postprocess_t postprocess;
} nuvk_sdf_pipelines_t;

void nuvk_sdf_graphics_pipeline_default_info(VkGraphicsPipelineCreateInfo *info);

nu_result_t nuvk_sdf_pipelines_initialize(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_renderpasses_t *renderpasses
);
nu_result_t nuvk_sdf_pipelines_terminate(
    nuvk_sdf_pipelines_t *pipelines,
    const nuvk_context_t *context
);

#endif