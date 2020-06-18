#include "command.h"

#include "../common/logger.h"
#include "../presentation/device.h"
#include "../presentation/physical_device.h"
#include "../presentation/swapchain.h"
#include "../pipeline/graphics_pipeline.h"
#include "../pipeline/render_pass.h"
#include "framebuffer.h"
#include "synchronization.h"

typedef struct {
    VkCommandPool command_pool;
    VkCommandBuffer *command_buffers;
    uint32_t command_buffer_count;
} nuvk_command_data_t;

static nuvk_command_data_t _data;

nu_result_t nuvk_command_pool_create(void)
{
    /* recover queue family indices from the physical device */
    nuvk_queue_family_indices_t indices;
    indices = nuvk_physical_device_get_queue_family_indices();

    /* create the pool info */
    VkCommandPoolCreateInfo pool_info;
    memset(&pool_info, 0, sizeof(VkCommandPoolCreateInfo));
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = indices.graphics_family;

    if (vkCreateCommandPool(nuvk_device_get_handle(), &pool_info, NULL, &_data.command_pool) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to create command pool.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_command_pool_destroy(void)
{
    /* destroy command pool */
    vkDestroyCommandPool(nuvk_device_get_handle(), _data.command_pool, NULL);

    return NU_SUCCESS;
}

nu_result_t nuvk_command_buffers_create(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* recover required swapchain info */
    VkExtent2D extent;
    extent = nuvk_swapchain_get_extent();

    /* recover framebuffers */
    const VkFramebuffer *framebuffers;
    uint32_t framebuffer_count;
    framebuffers = nuvk_framebuffer_get_swapchain(&framebuffer_count);

    /* create command buffers */
    _data.command_buffer_count = framebuffer_count; /* for each swapchain image */
    _data.command_buffers = (VkCommandBuffer*)nu_malloc(sizeof(VkCommandBuffer) * framebuffer_count);

    VkCommandBufferAllocateInfo alloc_info;
    memset(&alloc_info, 0, sizeof(VkCommandBufferAllocateInfo));
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = _data.command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = framebuffer_count;

    if (vkAllocateCommandBuffers(nuvk_device_get_handle(), &alloc_info, _data.command_buffers) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to allocate command buffers.\n");
        nu_free(_data.command_buffers);
        return NU_FAILURE;
    }

    /* record starting commands */
    for (uint32_t i = 0; i < _data.command_buffer_count; i++) {
        /* begin recording command */
        VkCommandBufferBeginInfo begin_info;
        memset(&begin_info, 0, sizeof(VkCommandBufferBeginInfo));
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(_data.command_buffers[i], &begin_info) != VK_SUCCESS) {
            nu_warning(NUVK_VULKAN_LOG_NAME"Failed to begin recording command buffer.\n");
            result = NU_FAILURE;
            break;
        }

        /* starting render pass */
        VkRenderPassBeginInfo render_pass_info;
        memset(&render_pass_info, 0, sizeof(VkRenderPassBeginInfo));
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = nuvk_render_pass_get_handle();
        render_pass_info.framebuffer = framebuffers[i];
        render_pass_info.renderArea.offset.x = 0;
        render_pass_info.renderArea.offset.y = 0;
        render_pass_info.renderArea.extent = extent;
        VkClearValue clear_color;
        clear_color.color.float32[0] = 0.0f;
        clear_color.color.float32[1] = 0.0f;
        clear_color.color.float32[2] = 0.0f;
        clear_color.color.float32[3] = 1.0f;
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;
        vkCmdBeginRenderPass(_data.command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        /* bind pipeline */
        vkCmdBindPipeline(_data.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, nuvk_graphics_pipeline_get_handle());

        /* draw triangle */
        vkCmdDraw(_data.command_buffers[i], 3, 1, 0, 0);

        /* finishing render pass */
        vkCmdEndRenderPass(_data.command_buffers[i]);

        /* end recording command */
        if (vkEndCommandBuffer(_data.command_buffers[i]) != VK_SUCCESS) {
            nu_warning(NUVK_VULKAN_LOG_NAME"Failed to end recording command buffer.\n");
            result = NU_FAILURE;
            break;
        }
    }

    return result;
}
nu_result_t nuvk_command_buffers_destroy(void)
{
    /* destroy command buffers */
    if (_data.command_buffers) {
        nu_free(_data.command_buffers);
    }

    return NU_SUCCESS;
}

const VkCommandBuffer *nuvk_command_buffers_get(uint32_t *count)
{
    *count = _data.command_buffer_count;
    return _data.command_buffers;
}