#ifndef NUVK_RENDER_CONTEXT_H
#define NUVK_RENDER_CONTEXT_H

#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/core/swapchain.h>
#include <nucleus/module/vulkan/core/command_pool.h>

typedef struct {
    uint32_t max_inflight_frame_count;
    uint32_t active_inflight_frame_index;
    uint32_t active_swapchain_image_index;

    VkSemaphore *image_available_semaphores;
    VkSemaphore *render_finished_semaphores;
    VkFence *inflight_fences;

    VkCommandBuffer *command_buffers;
} nuvk_render_context_t;

nu_result_t nuvk_render_context_initialize(
    nuvk_render_context_t *render_context,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_command_pool_t *command_pool,
    uint32_t max_inflight_frame_count
);
nu_result_t nuvk_render_context_terminate(
    nuvk_render_context_t *render_context,
    const nuvk_context_t *context
);

bool nuvk_render_context_begin(
    nuvk_render_context_t *render_context, 
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
);
bool nuvk_render_context_end(
    nuvk_render_context_t *render_context,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
);

#endif