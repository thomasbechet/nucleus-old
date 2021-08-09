#ifndef NUVK_SDF_SHADER_POSTPROCESS_H
#define NUVK_SDF_SHADER_POSTPROCESS_H

#include <nucleus/module/vulkan/core/shader.h>

typedef struct {
    VkShaderModule vertex;
    VkShaderModule fragment;
    VkPipelineShaderStageCreateInfo stages[2];
    uint32_t stage_count;
} nuvk_sdf_shader_postprocess_t;

nu_result_t nuvk_sdf_shader_postprocess_create(
    nuvk_sdf_shader_postprocess_t *shader,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager
);
nu_result_t nuvk_sdf_shader_postprocess_destroy(
    nuvk_sdf_shader_postprocess_t *shader,
    const nuvk_context_t *context
);

#endif