#ifndef NUVK_PIPELINE_LIGHT_H
#define NUVK_PIPELINE_LIGHT_H

#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/descriptor/descriptors.h>
#include <nucleus/module/vulkan/pipeline/generator.h>

typedef struct {
    VkShaderModule compute;
    VkPipelineLayout layout;
    VkPipeline pipeline;
} nuvk_pipeline_light_t;

nu_result_t nuvk_pipeline_light_create(
    nuvk_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_descriptors_t *descriptors,
    const nuvk_pipeline_generator_t *generator
);
nu_result_t nuvk_pipeline_light_destroy(
    nuvk_pipeline_light_t *pipeline,
    const nuvk_context_t *context
);
nu_result_t nuvk_pipeline_light_recompile(
    nuvk_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_descriptors_t *descriptors,
    const nuvk_pipeline_generator_t *generator
);

#endif