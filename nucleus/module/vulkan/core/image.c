#include <nucleus/module/vulkan/core/image.h>

nu_result_t nuvk_image_create(
    nuvk_image_t *image,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_image_info_t *info
)
{
    image->extent  = info->extent;
    image->format  = info->format;

    /* create image */
    VkImageCreateInfo image_info;
    memset(&image_info, 0, sizeof(VkImageCreateInfo));
    image_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType     = VK_IMAGE_TYPE_2D;
    image_info.extent.width  = info->extent.width;
    image_info.extent.height = info->extent.height;
    image_info.extent.depth  = 1;
    image_info.mipLevels     = 1;
    image_info.arrayLayers   = 1;
    image_info.format        = info->format;
    image_info.tiling        = info->tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage         = info->image_usage;
    image_info.samples       = VK_SAMPLE_COUNT_1_BIT;
    image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocation_info;
    memset(&allocation_info, 0, sizeof(VmaAllocationCreateInfo));
    allocation_info.usage = info->memory_usage;

    VkResult result = vmaCreateImage(memory_manager->allocator, &image_info, &allocation_info, &image->image, &image->allocation, NULL);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create image.");

    /* create image view */
    VkImageViewCreateInfo view_info;
    memset(&view_info, 0, sizeof(VkImageViewCreateInfo));
    view_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image                           = image->image;
    view_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format                          = info->format;
    view_info.subresourceRange.aspectMask     = info->aspectMask;
    view_info.subresourceRange.baseMipLevel   = 0;
    view_info.subresourceRange.levelCount     = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount     = 1;

    result = vkCreateImageView(context->device, &view_info, &context->allocator, &image->image_view);
    NU_CHECK(result == VK_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to create image view.");

    return NU_SUCCESS;

cleanup0:
    vmaDestroyImage(memory_manager->allocator, image->image, image->allocation);
    return NU_FAILURE;
}
nu_result_t nuvk_image_destroy(
    nuvk_image_t *image, 
    const nuvk_context_t *context, 
    const nuvk_memory_manager_t *memory_manager
)
{
    vkDestroyImageView(context->device, image->image_view, &context->allocator);
    vmaDestroyImage(memory_manager->allocator, image->image, image->allocation);

    return NU_SUCCESS;
}