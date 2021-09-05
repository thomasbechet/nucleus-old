#ifndef NUVK_SDF_PIPELINE_POSTPROCESS_H
#define NUVK_SDF_PIPELINE_POSTPROCESS_H

#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>

typedef struct {
    VkShaderModule vertex;
    VkShaderModule fragment;
    VkPipelineLayout layout;
    VkPipeline pipeline;
} nuvk_sdf_pipeline_postprocess_t;

nu_result_t nuvk_sdf_pipeline_postprocess_create(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    VkRenderPass postprocess_renderpass,
    const nu_string_t *sources
);
nu_result_t nuvk_sdf_pipeline_postprocess_destroy(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context
);

#endif