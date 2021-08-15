#ifndef NUVK_IMAGE_H
#define NUVK_IMAGE_H

#include <nucleus/module/vulkan/core/memory.h>

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

#endif