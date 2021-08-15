#ifndef NUVK_MEMORY_MANAGER_H
#define NUVK_MEMORY_MANAGER_H

#include <nucleus/module/vulkan/core/context.h>
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

typedef struct {
    VmaAllocator allocator;
} nuvk_memory_manager_t;

nu_result_t nuvk_memory_manager_initialize(
    nuvk_memory_manager_t *manager,
    const nuvk_context_t *context
);
nu_result_t nuvk_memory_manager_terminate(nuvk_memory_manager_t *manager);

#endif