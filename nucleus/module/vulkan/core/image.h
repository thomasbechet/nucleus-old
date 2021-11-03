#ifndef NUVK_IMAGE_H
#define NUVK_IMAGE_H

#include <nucleus/module/vulkan/core/memory.h>
#include <nucleus/module/vulkan/core/command_pool.h>

typedef struct {
    VkExtent2D extent;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlagBits image_usage;
    VkImageAspectFlagBits aspectMask;
    VmaMemoryUsage memory_usage;
} nuvk_image_info_t;

typedef struct {
    VkImage image;
    VkExtent2D extent;
    VkFormat format;
    VkImageSubresourceRange subresource_range;
    VkImageView image_view;
    VmaAllocation allocation;
} nuvk_image_t;

nu_result_t nuvk_image_create(
    nuvk_image_t *image, 
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_image_info_t *info
);
nu_result_t nuvk_image_destroy(
    nuvk_image_t *image,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
);

nu_result_t nuvk_image_transfer_layout(
    VkCommandBuffer cmd,
    const nuvk_image_t *image,
    VkImageLayout old_layout,
    VkImageLayout new_layout
);
nu_result_t nuvk_image_create_from_data(
    nuvk_image_t *image,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_command_pool_t *command_pool,
    const nuvk_image_info_t *info,
    VkImageLayout layout,
    uint32_t data_size,
    const uint8_t *data
);

#endif