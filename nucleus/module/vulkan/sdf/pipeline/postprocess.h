#ifndef NUVK_SDF_PIPELINE_POSTPROCESS_H
#define NUVK_SDF_PIPELINE_POSTPROCESS_H

#include <nucleus/module/vulkan/sdf/shader/postprocess.h>

nu_result_t nuvk_sdf_pipeline_postprocess_create(
    VkPipeline *pipeline,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shader_postprocess_t *shader,
    VkRenderPass postprocess_renderpass
);

#endif