#include <nucleus/module/vulkan/sdf/descriptor/pool.h>

nu_result_t nuvk_sdf_descriptor_pool_create(
    VkDescriptorPool *pool,
    const nuvk_context_t *context,
    uint32_t max_frame_inflight
)
{
    VkDescriptorPoolSize pool_sizes[1];
    memset(pool_sizes, 0, sizeof(VkDescriptorPoolSize) * 1);
    pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_sizes[0].descriptorCount = max_frame_inflight;

    VkDescriptorPoolCreateInfo info;
    memset(&info, 0, sizeof(VkDescriptorPoolCreateInfo));
    info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.poolSizeCount = 1;
    info.pPoolSizes    = pool_sizes;
    info.maxSets       = max_frame_inflight;

    if (vkCreateDescriptorPool(context->device, &info, &context->allocator, pool) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create descriptor pool.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}