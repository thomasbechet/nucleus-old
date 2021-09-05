#ifndef NUVK_SDF_PIPELINE_GEOMETRY_H
#define NUVK_SDF_PIPELINE_GEOMETRY_H

#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>

typedef struct {
    VkShaderModule vertex;
    VkShaderModule fragment;
    VkPipelineLayout layout;
    VkPipeline pipeline;
} nuvk_sdf_pipeline_geometry_t;

nu_result_t nuvk_sdf_pipeline_geometry_create(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    VkRenderPass geometry_renderpass,
    const nu_string_t *sources
);
nu_result_t nuvk_sdf_pipeline_geometry_destroy(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_pipeline_geometry_recompile(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    VkRenderPass geometry_renderpass,
    const nu_string_t *sources,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count
);

#endif