#ifndef NUVK_COMMAND_POOL_H
#define NUVK_COMMAND_POOL_H

#include <nucleus/module/vulkan/core/context.h>

typedef struct {
    VkCommandPool graphics_command_pool;
} nuvk_command_pool_t;

nu_result_t nuvk_command_pool_initialize(
    nuvk_command_pool_t *pool,
    const nuvk_context_t *context
);
nu_result_t nuvk_command_pool_terminate(nuvk_command_pool_t *pool, const nuvk_context_t *context);

nu_result_t nuvk_command_buffer_begin_single(VkDevice device, VkCommandPool command_pool, VkCommandBuffer *command_buffer);
nu_result_t nuvk_command_buffer_end_single(VkCommandBuffer command_buffer, VkCommandPool command_pool, VkQueue queue);
nu_result_t nuvk_command_buffer_create(VkDevice device, VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers);

#endif