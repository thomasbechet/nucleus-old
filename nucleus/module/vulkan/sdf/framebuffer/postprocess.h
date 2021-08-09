#ifndef NUVK_SDF_FRAMEBUFFER_POSTPROCESS_H
#define NUVK_SDF_FRAMEBUFFER_POSTPROCESS_H

#include <nucleus/module/vulkan/core/swapchain.h>
#include <nucleus/module/vulkan/core/render_context.h>

typedef struct {
    uint32_t framebuffer_count;
    VkFramebuffer *framebuffers;
} nuvk_sdf_framebuffer_postprocess_t;

nu_result_t nuvk_sdf_framebuffer_postprocess_create(
    nuvk_sdf_framebuffer_postprocess_t *framebuffer,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    VkRenderPass postprocess_renderpass
);
nu_result_t nuvk_sdf_framebuffer_postprocess_destroy(
    nuvk_sdf_framebuffer_postprocess_t *framebuffer,
    const nuvk_context_t *context
);

#endif