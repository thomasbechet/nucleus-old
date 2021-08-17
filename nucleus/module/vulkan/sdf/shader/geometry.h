#ifndef NUVK_SDF_SHADER_GEOMETRY_H
#define NUVK_SDF_SHADER_GEOMETRY_H

#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>

typedef struct {
    VkShaderModule vertex;
    VkShaderModule fragment;
    VkPipelineLayout layout;
    VkPipelineShaderStageCreateInfo stages[2];
    uint32_t stage_count;
} nuvk_sdf_shader_geometry_t;

nu_result_t nuvk_sdf_shader_geometry_create(
    nuvk_sdf_shader_geometry_t *shader,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors
);
nu_result_t nuvk_sdf_shader_geometry_destroy(
    nuvk_sdf_shader_geometry_t *shader,
    const nuvk_context_t *context
);

#endif