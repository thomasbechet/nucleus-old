#include <nucleus/module/vulkan/core/image.h>

#include <nucleus/module/vulkan/core/buffer.h>
#include <nucleus/module/vulkan/core/command_pool.h>

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

    image->subresource_range = view_info.subresourceRange;

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

nu_result_t nuvk_image_transfer_layout(
    VkCommandBuffer cmd,
    const nuvk_image_t *image,
    VkImageLayout old_layout,
    VkImageLayout new_layout
)
{
    VkImageMemoryBarrier barrier;
    memset(&barrier, 0, sizeof(VkImageMemoryBarrier));
    barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout           = old_layout;
    barrier.newLayout           = new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image               = image->image;
    barrier.subresourceRange    = image->subresource_range;
    
    VkPipelineStageFlags source_stage, destination_stage;
    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        source_stage          = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage     = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        source_stage          = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage     = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        nu_warning(NUVK_LOGGER_NAME, "Unsupported layout transition.");
    }

    vkCmdPipelineBarrier(cmd, source_stage, destination_stage, 0, 0, NULL, 0, NULL, 1, &barrier);

    return NU_SUCCESS;
}
nu_result_t nuvk_image_create_from_data(
    nuvk_image_t *image,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_command_pool_t *command_pool,
    const nuvk_image_info_t *info,
    VkImageLayout layout,
    uint32_t data_size,
    const uint8_t *data
)
{
    nu_result_t result = NU_SUCCESS;

    /* create stating buffer */
    nuvk_buffer_t staging_buffer;
    nuvk_buffer_info_t staging_buffer_info;
    memset(&staging_buffer_info, 0, sizeof(nuvk_buffer_info_t));
    staging_buffer_info.buffer_usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    staging_buffer_info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    staging_buffer_info.size         = data_size;
    result = nuvk_buffer_create(&staging_buffer, memory_manager, &staging_buffer_info);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create staging buffer.");
    result = nuvk_buffer_map(&staging_buffer, memory_manager);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to map staging buffer.");
    memcpy(staging_buffer.map, data, data_size);
    nuvk_buffer_unmap(&staging_buffer, memory_manager);

    /* create image */
    nuvk_image_info_t image_info = *info;
    image_info.image_usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    result = nuvk_image_create(image, context, memory_manager, &image_info);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to create image.");

    /* copy */
    VkCommandBuffer cmd;
    result = nuvk_command_buffer_begin_single(context->device, command_pool->graphics_command_pool, &cmd);
    NU_CHECK(result == NU_SUCCESS, goto cleanup1, NUVK_LOGGER_NAME, "Failed to create command buffer.");

    /* transfer image to dst optimal */
    nuvk_image_transfer_layout(cmd, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    /* copy buffer to image */
    VkBufferImageCopy region;
    region.bufferOffset                    = 0;
    region.bufferRowLength                 = 0;
    region.bufferImageHeight               = 0;
    region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel       = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount     = 1;
    region.imageOffset.x                   = 0;
    region.imageOffset.y                   = 0;
    region.imageOffset.z                   = 0;
    region.imageExtent.width               = info->extent.width;
    region.imageExtent.height              = info->extent.height;
    region.imageExtent.depth               = 1;

    vkCmdCopyBufferToImage(cmd, staging_buffer.buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    
    /* transfer image to layout */
    nuvk_image_transfer_layout(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, layout);

    /* send commands */
    result = nuvk_command_buffer_end_single(cmd, command_pool->compute_command_pool, context->queues.graphics_compute);
    NU_CHECK(result == NU_SUCCESS, goto cleanup1, NUVK_LOGGER_NAME, "Failed to end command buffer.");

    nuvk_buffer_destroy(&staging_buffer, memory_manager);

    return result;

cleanup1:
    nuvk_image_destroy(image, context, memory_manager);
cleanup0:
    nuvk_buffer_destroy(&staging_buffer, memory_manager);
    return result;
}