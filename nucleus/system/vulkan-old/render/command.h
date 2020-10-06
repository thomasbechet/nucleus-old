#ifndef NUVK_COMMAND_H
#define NUVK_COMMAND_H

#include "../renderer.h"

nu_result_t nuvk_command_pool_create(VkCommandPool *command_pool);
nu_result_t nuvk_command_pool_destroy(VkCommandPool command_pool);

nu_result_t nuvk_command_buffers_allocate(
    VkCommandBuffer *command_buffers,
    uint32_t command_buffer_count,
    VkCommandPool command_pool
);

#endif