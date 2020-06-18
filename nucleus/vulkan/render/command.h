#ifndef NUVK_COMMAND_H
#define NUVK_COMMAND_H

#include "../renderer.h"

nu_result_t nuvk_command_pool_create(void);
nu_result_t nuvk_command_pool_destroy(void);

nu_result_t nuvk_command_buffers_create(void);
nu_result_t nuvk_command_buffers_destroy(void);

const VkCommandBuffer *nuvk_command_buffers_get(uint32_t *count);

#endif