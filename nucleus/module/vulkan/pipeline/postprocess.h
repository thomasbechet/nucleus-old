#ifndef NUVK_PIPELINE_POSTPROCESS_H
#define NUVK_PIPELINE_POSTPROCESS_H

#include <nucleus/module/vulkan/core/pipeline.h>
#include <nucleus/module/vulkan/core/renderpass.h>
#include <nucleus/module/vulkan/core/swapchain.h>

nu_result_t nuvk_pipeline_postprocess_create(
    nuvk_pipeline_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_renderpass_t *postprocess_renderpass
);

#endif