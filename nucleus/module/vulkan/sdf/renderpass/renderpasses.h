#ifndef NUVK_SDF_RENDERPASSES_H
#define NUVK_SDF_RENDERPASSES_H

#include <nucleus/module/vulkan/core/swapchain.h>

typedef struct {
    VkRenderPass postprocess;
} nuvk_sdf_renderpasses_t;

nu_result_t nuvk_sdf_renderpasses_initialize(
    nuvk_sdf_renderpasses_t *renderpasses,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
);
nu_result_t nuvk_sdf_renderpasses_terminate(
    nuvk_sdf_renderpasses_t *renderpasses,
    const nuvk_context_t *context
);

#endif