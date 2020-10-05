#ifndef NUVK_PIPELINE_H
#define NUVK_PIPELINE_H

#include "../renderer.h"

nu_result_t nuvk_pipeline_layout_create_default(VkPipelineLayout *layout);
nu_result_t nuvk_pipleine_layout_destroy(VkPipelineLayout layout);

nu_result_t nuvk_pipeline_create_default(
    VkPipeline *pipeline,
    VkShaderModule vertex_shader_module,
    VkShaderModule fragment_shader_module,
    VkPipelineLayout layout,
    VkRenderPass render_pass
);
nu_result_t nuvk_pipeline_destroy(VkPipeline pipeline);

#endif