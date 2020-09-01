#ifndef NUVK_SYNCHRONIZATION_H
#define NUVK_SYNCHRONIZATION_H

#include "../renderer.h"

nu_result_t nuvk_semaphore_create(VkSemaphore *semaphore);
nu_result_t nuvk_semaphore_destroy(VkSemaphore semaphore);
nu_result_t nuvk_fence_create(VkFence *fence);
nu_result_t nuvk_fence_destroy(VkFence fence);

#endif