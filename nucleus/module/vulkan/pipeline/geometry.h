#ifndef NUVK_PIPELINE_GEOMETRY_H
#define NUVK_PIPELINE_GEOMETRY_H

#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/descriptor/descriptors.h>
#include <nucleus/module/vulkan/pipeline/generator.h>

typedef struct {
    VkShaderModule vertex;
    VkShaderModule fragment;
    VkPipelineLayout layout;
    VkPipeline pipeline;
} nuvk_pipeline_geometry_t;

nu_result_t nuvk_pipeline_geometry_create(
    nuvk_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_descriptors_t *descriptors,
    VkRenderPass geometry_renderpass,
    const nuvk_pipeline_generator_t *generator
);
nu_result_t nuvk_pipeline_geometry_destroy(
    nuvk_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context
);
nu_result_t nuvk_pipeline_geometry_recompile(
    nuvk_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    VkRenderPass geometry_renderpass,
    const nuvk_pipeline_generator_t *generator
);

#endif