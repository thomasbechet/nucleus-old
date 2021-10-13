#ifndef NUVK_SDF_PIPELINE_LIGHT_H
#define NUVK_SDF_PIPELINE_LIGHT_H

#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>
#include <nucleus/module/vulkan/sdf/pipeline/generator.h>

typedef struct {
    VkShaderModule compute;
    VkPipelineLayout layout;
    VkPipeline pipeline;
} nuvk_sdf_pipeline_light_t;

nu_result_t nuvk_sdf_pipeline_light_create(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_pipeline_generator_t *generator
);
nu_result_t nuvk_sdf_pipeline_light_destroy(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_pipeline_light_recompile(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_pipeline_generator_t *generator
);

#endif