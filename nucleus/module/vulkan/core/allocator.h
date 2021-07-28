#ifndef NUVK_ALLOCATOR_H
#define NUVK_ALLOCATOR_H

#include <nucleus/nucleus.h>
#include <vulkan/vulkan.h>

nu_result_t nuvk_allocator_initialize(VkAllocationCallbacks *allocator);

#endif