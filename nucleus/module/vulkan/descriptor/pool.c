#include <nucleus/module/vulkan/descriptor/pool.h>

nu_result_t nuvk_descriptor_pool_create(
    VkDescriptorPool *pool,
    const nuvk_context_t *context,
    uint32_t max_frame_inflight
)
{
    VkDescriptorPoolSize pool_sizes[2];
    memset(pool_sizes, 0, sizeof(VkDescriptorPoolSize) * 2);
    pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    pool_sizes[0].descriptorCount = 4;
    pool_sizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    pool_sizes[1].descriptorCount = 4;

    VkDescriptorPoolCreateInfo info;
    memset(&info, 0, sizeof(VkDescriptorPoolCreateInfo));
    info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.poolSizeCount = 2;
    info.pPoolSizes    = pool_sizes;
    info.maxSets       = 3;

    VkResult result = vkCreateDescriptorPool(context->device, &info, &context->allocator, pool);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create descriptor pool.");

    return NU_SUCCESS;
}