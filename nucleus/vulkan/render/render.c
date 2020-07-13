#include "render.h"

#include "../common/logger.h"
#include "../context/context.h"
#include "render_pass.h"
#include "shader.h"
#include "pipeline.h"
#include "framebuffer.h"
#include "command.h"
#include "synchronization.h"

#define NUVK_FRAME_RESOURCES_COUNT 2

typedef struct {
    VkSemaphore image_available;
    VkSemaphore render_finished;
    VkFence fence;
} nuvk_frame_synchronization_t;

typedef struct {
    /* renderpass */
    VkRenderPass render_pass;

    /* pipeline */
    VkShaderModule fragment_shader_module;
    VkShaderModule vertex_shader_module;
    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;

    /* framebuffers */
    VkFramebuffer *framebuffers;
    uint32_t framebuffer_count;

    /* commands */
    VkCommandPool command_pool;
    VkCommandBuffer *command_buffers;
    uint32_t command_buffer_count;

    /* synchronization objects */
    nuvk_frame_synchronization_t frame_sync[NUVK_FRAME_RESOURCES_COUNT];
    uint32_t image_fence_count;
    VkFence *image_fences;

    /* frame info */
    uint32_t current_frame;
} nuvk_render_data_t;

static nuvk_render_data_t _data;

static nu_result_t create_render_pass(void)
{
    if (nuvk_render_pass_create_default(&_data.render_pass) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create render pass.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
static nu_result_t destroy_render_pass(void)
{
    if (nuvk_render_pass_destroy(_data.render_pass) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to destroy render pass.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static nu_result_t create_pipeline(void)
{   
    if (nuvk_shader_module_create(&_data.vertex_shader_module, "engine/shaders/shader.vert.spv") != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create vertex shader module.\n");
        return NU_FAILURE;
    }
    if (nuvk_shader_module_create(&_data.fragment_shader_module, "engine/shaders/shader.frag.spv") != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create fragment shader module.\n");
        return NU_FAILURE;
    }
    if (nuvk_pipeline_layout_create_default(&_data.pipeline_layout) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create pipeline layout.\n");
        return NU_FAILURE;
    }
    if (nuvk_pipeline_create_default(
        &_data.pipeline, 
        _data.vertex_shader_module, 
        _data.fragment_shader_module,
        _data.pipeline_layout,
        _data.render_pass) != NU_SUCCESS)
    {
        nu_warning(NUVK_LOGGER_NAME"Failed to create pipeline.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
static nu_result_t destroy_pipeline(void)
{
    if (nuvk_pipeline_destroy(_data.pipeline) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to destroy pipeline.\n");
        return NU_FAILURE;
    }
    if (nuvk_pipleine_layout_destroy(_data.pipeline_layout) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to destroy pipeline layout.\n");
        return NU_FAILURE;
    }
    if (nuvk_shader_module_destroy(_data.vertex_shader_module) != NU_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to destroy vertex shader module.\n");
        return NU_FAILURE;
    }
    if (nuvk_shader_module_destroy(_data.fragment_shader_module) != NU_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to destroy fragment shader module.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static nu_result_t create_framebuffers(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    VkExtent2D extent = ctx->swapchain.extent;
    VkImageView *images = ctx->swapchain.images;

    _data.framebuffer_count = ctx->swapchain.image_count;
    _data.framebuffers = (VkFramebuffer*)nu_malloc(sizeof(VkFramebuffer) * _data.framebuffer_count);
    for (uint32_t i = 0; i < _data.framebuffer_count; i++) {
        if (nuvk_framebuffer_create(&_data.framebuffers[i], &images[i], 1, extent, _data.render_pass) != NU_SUCCESS) {
            nu_warning(NUVK_LOGGER_NAME"Failed to create framebuffer.\n");
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}
static nu_result_t destroy_framebuffers(void)
{
    for (uint32_t i = 0; i < _data.framebuffer_count; i++) {
        if (nuvk_framebuffer_destroy(_data.framebuffers[i]) != NU_SUCCESS) {
            nu_warning(NUVK_LOGGER_NAME"Failed to destroy framebuffer.\n");
            return NU_FAILURE;
        }
    }
    nu_free(_data.framebuffers);

    return NU_SUCCESS;
}

static nu_result_t create_commands(void)
{
    if (nuvk_command_pool_create(&_data.command_pool) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create command pool.\n");
    }

    _data.command_buffer_count = _data.framebuffer_count;
    _data.command_buffers = (VkCommandBuffer*)nu_malloc(sizeof(VkCommandBuffer) * _data.command_buffer_count);
    if (nuvk_command_buffers_allocate(
        _data.command_buffers,
        _data.command_buffer_count,
        _data.command_pool
    ) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to allocate command buffers.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
static nu_result_t destroy_commands(void)
{
    nu_free(_data.command_buffers);

    if (nuvk_command_pool_destroy(_data.command_pool) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to destroy command pool.\n");
    }

    return NU_SUCCESS;
}

static nu_result_t record_command_buffers(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    for (uint32_t i = 0; i < _data.command_buffer_count; i++) {
        /* begin recording command */
        VkCommandBufferBeginInfo begin_info;
        memset(&begin_info, 0, sizeof(VkCommandBufferBeginInfo));
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(_data.command_buffers[i], &begin_info) != VK_SUCCESS) {
            nu_warning(NUVK_LOGGER_NAME"Failed to begin recording command buffer.\n");
            break;
        }

        /* starting render pass */
        VkRenderPassBeginInfo render_pass_info;
        memset(&render_pass_info, 0, sizeof(VkRenderPassBeginInfo));
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = _data.render_pass;
        render_pass_info.framebuffer = _data.framebuffers[i];
        render_pass_info.renderArea.offset.x = 0;
        render_pass_info.renderArea.offset.y = 0;
        render_pass_info.renderArea.extent = ctx->swapchain.extent;
        VkClearValue clear_color;
        clear_color.color.float32[0] = 0.0f;
        clear_color.color.float32[1] = 0.0f;
        clear_color.color.float32[2] = 0.0f;
        clear_color.color.float32[3] = 1.0f;
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;
        vkCmdBeginRenderPass(_data.command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        /* bind pipeline */
        vkCmdBindPipeline(_data.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _data.pipeline);

        /* draw triangle */
        vkCmdDraw(_data.command_buffers[i], 3, 1, 0, 0);

        /* finishing render pass */
        vkCmdEndRenderPass(_data.command_buffers[i]);

        /* end recording command */
        if (vkEndCommandBuffer(_data.command_buffers[i]) != VK_SUCCESS) {
            nu_warning(NUVK_LOGGER_NAME"Failed to end recording command buffer.\n");
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}

static nu_result_t create_synchronization_objects(void)
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

    return NU_SUCCESS;
}
static nu_result_t destroy_synchronization_objects(void)
{
    /* image fences */
    nu_free(_data.image_fences);

    /* frame synchronization objects */
    for (uint32_t i = 0; i < NUVK_FRAME_RESOURCES_COUNT; i++) {
        nuvk_semaphore_destroy(_data.frame_sync[i].image_available);
        nuvk_semaphore_destroy(_data.frame_sync[i].render_finished);
        nuvk_fence_destroy(_data.frame_sync[i].fence);
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_render_create(void)
{
    /* initialize render */
    nu_info(NUVK_LOGGER_NAME"Render -- Creating render pass...\n");
    if (create_render_pass() != NU_SUCCESS) return NU_FAILURE;
    nu_info(NUVK_LOGGER_NAME"Render -- Creating pipeline...\n");
    if (create_pipeline() != NU_SUCCESS) return NU_FAILURE;
    nu_info(NUVK_LOGGER_NAME"Render -- Creating framebuffers...\n");
    if (create_framebuffers() != NU_SUCCESS) return NU_FAILURE;
    nu_info(NUVK_LOGGER_NAME"Render -- Creating commands...\n");
    if (create_commands() != NU_SUCCESS) return NU_FAILURE;
    nu_info(NUVK_LOGGER_NAME"Render -- Recording command buffers...\n");
    if (record_command_buffers() != NU_SUCCESS) return NU_FAILURE;
    nu_info(NUVK_LOGGER_NAME"Render -- Creating synchronization objects...\n");
    if (create_synchronization_objects() != NU_SUCCESS) return NU_FAILURE;

    /* initialize current frame index */
    _data.current_frame = 0;

    return NU_SUCCESS;
}
nu_result_t nuvk_render_destroy(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    /* wait device idle */
    vkDeviceWaitIdle(ctx->device);

    /* terminate render */
    if (destroy_synchronization_objects() != NU_SUCCESS) return NU_FAILURE;
    if (destroy_commands() != NU_SUCCESS) return NU_FAILURE;
    if (destroy_framebuffers() != NU_SUCCESS) return NU_FAILURE;
    if (destroy_pipeline() != NU_SUCCESS) return NU_FAILURE;
    if (destroy_render_pass() != NU_SUCCESS) return NU_FAILURE;

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

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &_data.command_buffers[image_index];

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