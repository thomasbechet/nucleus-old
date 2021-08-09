#ifndef NUVK_SDF_PIPELINE_POSTPROCESS_H
#define NUVK_SDF_PIPELINE_POSTPROCESS_H

#include <nucleus/module/vulkan/core/swapchain.h>
#include <nucleus/module/vulkan/sdf/shader/postprocess.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>

typedef struct {
    VkPipelineLayout layout;
    VkPipeline pipeline;
} nuvk_sdf_pipeline_postprocess_t;

nu_result_t nuvk_sdf_pipeline_postprocess_create(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shader_postprocess_t *shader,
    const nuvk_sdf_descriptors_t *descriptors,
    VkRenderPass postprocess_renderpass
);
nu_result_t nuvk_sdf_pipeline_postprocess_destroy(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context
);

#endif