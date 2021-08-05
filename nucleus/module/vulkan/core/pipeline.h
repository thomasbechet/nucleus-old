#ifndef NUVK_PIPELINE_H
#define NUVK_PIPELINE_H

#include <nucleus/module/vulkan/core/context.h>

typedef struct {
    VkPipeline pipeline;
    VkPipelineLayout layout;
} nuvk_pipeline_t;

nu_result_t nuvk_pipeline_create_graphics(
    nuvk_pipeline_t *pipeline,
    const nuvk_context_t *context,
    const VkPipelineLayoutCreateInfo *layout_info,
    const VkGraphicsPipelineCreateInfo *pipeline_info
);
nu_result_t nuvk_pipeline_destroy(
    nuvk_pipeline_t *pipeline,
    const nuvk_context_t *context
);

#endif