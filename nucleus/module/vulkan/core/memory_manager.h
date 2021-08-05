#ifndef NUVK_MEMORY_MANAGER_H
#define NUVK_MEMORY_MANAGER_H

#include <nucleus/module/vulkan/core/context.h>
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

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

typedef struct {
    VmaAllocator allocator;
} nuvk_memory_manager_t;

nu_result_t nuvk_memory_manager_initialize(
    nuvk_memory_manager_t *manager,
    const nuvk_context_t *context
);
nu_result_t nuvk_memory_manager_terminate(nuvk_memory_manager_t *manager);

nu_result_t nuvk_buffer_create(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager, const nuvk_buffer_info_t *info);
nu_result_t nuvk_buffer_destroy(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager);
nu_result_t nuvk_buffer_map(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager);
nu_result_t nuvk_buffer_unmap(nuvk_buffer_t *buffer, const nuvk_memory_manager_t *manager);

#endif