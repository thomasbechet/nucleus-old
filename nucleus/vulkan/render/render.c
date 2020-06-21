#include "render.h"

#include "../common/logger.h"
#include "../context/context.h"
#include "command.h"
#include "synchronization.h"

#define NUVK_FRAME_RESOURCES_COUNT 2

typedef struct {
    VkSemaphore image_available;
    VkSemaphore render_finished;
    VkFence fence;
} nuvk_frame_synchronization_t;

typedef struct {
    /* frame info */
    uint32_t current_frame;

    /* synchronization objects */
    nuvk_frame_synchronization_t frame_sync[NUVK_FRAME_RESOURCES_COUNT];

    uint32_t image_fence_count;
    VkFence *image_fences;
} nuvk_render_data_t;

static nuvk_render_data_t _data;

static void create_synchronization_objects(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    /* frame synchronization objects */
    for (uint32_t i = 0; i < NUVK_FRAME_RESOURCES_COUNT; i++) {
        nuvk_semaphore_create(&_data.frame_sync[i].image_available);
        nuvk_semaphore_create(&_data.frame_sync[i].render_finished);
        nuvk_fence_create(&_data.frame_sync[i].fence);
    }

    /* image fences */
    _data.image_fence_count = ctx->swapchain.image_count;
    _data.image_fences = (VkFence*)nu_malloc(sizeof(VkFence) * _data.image_fence_count);
    for (uint32_t i = 0; i < _data.image_fence_count; i++) {
        _data.image_fences[i] = VK_NULL_HANDLE;
    }
}
static void destroy_synchronization_objects(void)
{
    /* image fences */
    nu_free(_data.image_fences);

    /* frame synchronization objects */
    for (uint32_t i = 0; i < NUVK_FRAME_RESOURCES_COUNT; i++) {
        nuvk_semaphore_destroy(_data.frame_sync[i].image_available);
        nuvk_semaphore_destroy(_data.frame_sync[i].render_finished);
        nuvk_fence_destroy(_data.frame_sync[i].fence);
    }
}

nu_result_t nuvk_render_create(void)
{
    /* create synchronization objects */
    create_synchronization_objects();

    /* initialize current frame index */
    _data.current_frame = 0;

    return NU_SUCCESS;
}
nu_result_t nuvk_render_destroy(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    /* wait device idle */
    vkDeviceWaitIdle(ctx->device);

    /* destroy synchronization objects */
    destroy_synchronization_objects();

    return NU_SUCCESS;
}

nu_result_t nuvk_render_draw(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    /* wait fence */
    vkWaitForFences(ctx->device, 1, &_data.frame_sync[_data.current_frame].fence, VK_TRUE, UINT64_MAX);
    vkResetFences(ctx->device, 1, &_data.frame_sync[_data.current_frame].fence);

    /* acquire next image index */
    uint32_t image_index;
    vkAcquireNextImageKHR(ctx->device, ctx->swapchain.handle, UINT64_MAX,
        _data.frame_sync[_data.current_frame].image_available, VK_NULL_HANDLE, &image_index);

    if (_data.image_fences[image_index] != VK_NULL_HANDLE) {
        vkWaitForFences(ctx->device, 1, &_data.image_fences[image_index], VK_TRUE, UINT64_MAX);
    }

    _data.image_fences[image_index] = _data.frame_sync[_data.current_frame].fence;

    /* acquire synchronization objects */
    VkSemaphore wait_semaphores[] = {_data.frame_sync[_data.current_frame].image_available};
    VkSemaphore signal_semaphore[] = {_data.frame_sync[_data.current_frame].render_finished};

    /* submit command buffers */
    VkSubmitInfo submit_info;
    memset(&submit_info, 0, sizeof(VkSubmitInfo));
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.pWaitDstStageMask = wait_stages;

    uint32_t command_buffer_count;
    const VkCommandBuffer *command_buffers;
    command_buffers = nuvk_command_buffers_get(&command_buffer_count);
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffers[image_index];

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphore;

    vkResetFences(ctx->device, 1, &_data.frame_sync[_data.current_frame].fence);
    if (vkQueueSubmit(ctx->queues.graphics,
        1, &submit_info, _data.frame_sync[_data.current_frame].fence) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to submit draw command buffer.\n");
    }

    /* present frame */
    VkPresentInfoKHR present_info;
    memset(&present_info, 0, sizeof(VkPresentInfoKHR));
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphore;

    VkSwapchainKHR swapchains[] = {ctx->swapchain.handle};
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &image_index;

    /* present image */
    vkQueuePresentKHR(ctx->queues.presentation, &present_info);

    /* advance next frame */
    _data.current_frame = (_data.current_frame + 1) % NUVK_FRAME_RESOURCES_COUNT;

    return NU_SUCCESS;
}