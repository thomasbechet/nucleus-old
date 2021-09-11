#include <nucleus/module/vulkan/core/render_context.h>

nu_result_t nuvk_render_context_initialize(
    nuvk_render_context_t *render_context,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_command_pool_t *command_pool,
    uint32_t max_inflight_frame_count
)
{
    /* initialize values */
    render_context->active_inflight_frame_index  = 0;
    render_context->active_swapchain_image_index = 0;

    /* allocate memory */
    render_context->max_inflight_frame_count   = max_inflight_frame_count;
    render_context->image_available_semaphores = (VkSemaphore*)nu_malloc(sizeof(VkSemaphore) * max_inflight_frame_count);
    render_context->render_finished_semaphores = (VkSemaphore*)nu_malloc(sizeof(VkSemaphore) * max_inflight_frame_count);
    render_context->inflight_fences            = (VkFence*)nu_malloc(sizeof(VkFence) * max_inflight_frame_count);
    render_context->command_buffers            = (VkCommandBuffer*)nu_malloc(sizeof(VkCommandBuffer) * max_inflight_frame_count);

    /* create command buffers */
    nuvk_command_buffer_create(context->device, command_pool->graphics_command_pool, max_inflight_frame_count, render_context->command_buffers);

    /* create synchronization objects */
    VkSemaphoreCreateInfo semaphore_info;
    memset(&semaphore_info, 0, sizeof(VkSemaphoreCreateInfo));
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fence_info;
    memset(&fence_info, 0, sizeof(VkFenceCreateInfo));
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32_t i = 0; i < max_inflight_frame_count; i++) {
        vkCreateSemaphore(context->device, &semaphore_info, &context->allocator, &render_context->image_available_semaphores[i]);
        vkCreateSemaphore(context->device, &semaphore_info, &context->allocator, &render_context->render_finished_semaphores[i]);
        vkCreateFence(context->device, &fence_info, &context->allocator, &render_context->inflight_fences[i]);
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_render_context_terminate(
    nuvk_render_context_t *render_context,
    const nuvk_context_t *context
)
{
    vkDeviceWaitIdle(context->device);

    /* destroy synchronization objects */
    for (uint32_t i = 0; i < render_context->max_inflight_frame_count; i++) {
        vkDestroySemaphore(context->device, render_context->image_available_semaphores[i], &context->allocator);
        vkDestroySemaphore(context->device, render_context->render_finished_semaphores[i], &context->allocator);
        vkDestroyFence(context->device, render_context->inflight_fences[i], &context->allocator);
    }

    /* free memory */
    nu_free(render_context->image_available_semaphores);
    nu_free(render_context->render_finished_semaphores);
    nu_free(render_context->inflight_fences);
    nu_free(render_context->command_buffers);

    return NU_SUCCESS;
}

bool nuvk_render_context_begin(
    nuvk_render_context_t *render_context, 
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
)
{
    /* acquire next frame resource */
    VkFence inflight_fence                = render_context->inflight_fences[render_context->active_inflight_frame_index];
    VkSemaphore image_available_semaphore = render_context->image_available_semaphores[render_context->active_inflight_frame_index];

    if (vkWaitForFences(context->device, 1, &inflight_fence, VK_TRUE, UINT64_MAX) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to wait fence.");
        return false;
    }
    vkResetFences(context->device, 1, &inflight_fence);

    VkResult result = vkAcquireNextImageKHR(context->device, swapchain->swapchain, UINT64_MAX,
        image_available_semaphore, VK_NULL_HANDLE, &render_context->active_swapchain_image_index);
    if (result != VK_SUCCESS) {
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            return false;
        } else {
            nu_error(NUVK_LOGGER_NAME, "Failed to acquire next image KHR.");
            return false;
        }
    }

    /* begin command buffer */
    VkCommandBuffer cmd = render_context->command_buffers[render_context->active_inflight_frame_index];
    vkResetCommandBuffer(cmd, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

    VkCommandBufferBeginInfo begin_info;
    memset(&begin_info, 0, sizeof(VkCommandBufferBeginInfo));
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(cmd, &begin_info) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to begin command buffer.");
        return false;
    }

    return true;
}
bool nuvk_render_context_end(
    nuvk_render_context_t *render_context,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain
)
{
    /* end command buffer */
    VkCommandBuffer cmd = render_context->command_buffers[render_context->active_inflight_frame_index];
    vkEndCommandBuffer(cmd);

    /* submit command buffer */
    VkFence inflight_fence                = render_context->inflight_fences[render_context->active_inflight_frame_index];
    VkSemaphore image_available_semaphore = render_context->image_available_semaphores[render_context->active_inflight_frame_index];
    VkSemaphore render_finished_semaphore = render_context->render_finished_semaphores[render_context->active_inflight_frame_index];

    VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info;
    memset(&submit_info, 0, sizeof(VkSubmitInfo));
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount   = 1;
    submit_info.pWaitSemaphores      = &image_available_semaphore;
    submit_info.pWaitDstStageMask    = &dstStageMask;
    submit_info.commandBufferCount   = 1;
    submit_info.pCommandBuffers      = &cmd;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores    = &render_finished_semaphore;

    if (vkQueueSubmit(context->queues.graphics_compute, 1, &submit_info, inflight_fence) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to submit command buffer.");
        return false;
    }

    VkPresentInfoKHR present_info;
    memset(&present_info, 0, sizeof(VkPresentInfoKHR));
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores    = &render_finished_semaphore;
    present_info.swapchainCount     = 1;
    present_info.pSwapchains        = &swapchain->swapchain;
    present_info.pImageIndices      = &render_context->active_swapchain_image_index;

    VkResult result = vkQueuePresentKHR(context->queues.present, &present_info);
    if (result != VK_SUCCESS) {
        if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
            return false;
        } else {
            nu_error(NUVK_LOGGER_NAME, "Failed to present KHR.");
            return false;
        }
    }

    /* next active frame */
    render_context->active_inflight_frame_index = (render_context->active_inflight_frame_index + 1) % render_context->max_inflight_frame_count;

    return true;
}