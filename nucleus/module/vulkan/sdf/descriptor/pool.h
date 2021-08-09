#ifndef NUVK_SDF_DESCRIPTOR_POOL_H
#define NUVK_SDF_DESCRIPTOR_POOL_H

#include <nucleus/module/vulkan/core/context.h>

nu_result_t nuvk_sdf_descriptor_pool_create(
    VkDescriptorPool *pool,
    const nuvk_context_t *context,
    uint32_t max_frame_inflight
);

#endif