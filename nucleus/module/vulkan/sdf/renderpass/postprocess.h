#ifndef NUVK_SDF_RENDERPASS_POSTPROCESS_H
#define NUVK_SDF_RENDERPASS_POSTPROCESS_H

#include <nucleus/module/vulkan/core/swapchain.h>

nu_result_t nuvk_sdf_renderpass_postprocess_create(
    VkRenderPass *renderpass,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
);

#endif