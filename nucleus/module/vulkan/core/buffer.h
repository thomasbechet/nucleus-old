#ifndef NUVK_BUFFER_H
#define NUVK_BUFFER_H

#include <nucleus/module/vulkan/core/memory.h>

typedef struct {
    uint32_t size;
    VmaMemoryUsage memory_usage;
    VkBufferUsageFlagBits buffer_usage;
} nuvk_buffer_info_t;

typedef struct {
    VmaAllocation allocation;
    VkBuffer buffer;
    void *map;
} nuvk_buffer_t;

nu_result_t nuvk_buffer_create(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager, const nuvk_buffer_info_t *info);
nu_result_t nuvk_buffer_destroy(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager);
nu_result_t nuvk_buffer_map(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager);
nu_result_t nuvk_buffer_unmap(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager);

#endif