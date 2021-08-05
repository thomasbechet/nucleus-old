#ifndef NUVK_RENDERPASS_H
#define NUVK_RENDERPASS_H

#include <nucleus/module/vulkan/core/context.h>

typedef struct {
    VkRenderPass renderpass;
} nuvk_renderpass_t;

nu_result_t nuvk_renderpass_create(
    nuvk_renderpass_t *renderpass,
    const nuvk_context_t *context,
    const VkRenderPassCreateInfo *info
);
nu_result_t nuvk_renderpass_destroy(
    nuvk_renderpass_t *renderpass,
    const nuvk_context_t *context
);

#endif