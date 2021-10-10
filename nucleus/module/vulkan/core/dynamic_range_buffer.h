#ifndef NUVK_DYNAMIC_RANGE_BUFFER_H
#define NUVK_DYNAMIC_RANGE_BUFFER_H

#include <nucleus/module/vulkan/core/buffer.h>

typedef struct {
    uint32_t range_size;
    uint32_t block_size;
    uint32_t block_count;
    uint8_t *update_table;
    void *data;
    nuvk_buffer_t buffer;
} nuvk_dynamic_range_buffer_t;

nu_result_t nuvk_dynamic_range_buffer_create(
    nuvk_dynamic_range_buffer_t *buffer,
    const nuvk_memory_manager_t *memory_manager,
    VkBufferUsageFlagBits buffer_usage,
    uint32_t range_size,
    uint32_t range_count,
    uint32_t block_size
);
nu_result_t nuvk_dynamic_range_buffer_destroy(
    nuvk_dynamic_range_buffer_t *buffer,
    const nuvk_memory_manager_t *memory_manager
);

nu_result_t nuvk_dynamic_range_buffer_write(
    nuvk_dynamic_range_buffer_t *buffer,
    uint32_t range_index,
    uint32_t offset,
    uint32_t size,
    const void *data
);
nu_result_t nuvk_dynamic_range_buffer_transfer(
    nuvk_dynamic_range_buffer_t *buffer,
    uint32_t range_index
);

#endif