#include <nucleus/module/vulkan/sdf/image/geometry.h>

nu_result_t nuvk_sdf_image_geometry_create(
    nuvk_sdf_image_geometry_t *geometry,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
)
{
    nuvk_image_info_t info;
    memset(&info, 0, sizeof(nuvk_image_info_t));
    info.extent.width  = swapchain->extent.width;
    info.extent.height = swapchain->extent.height;
    info.format        = VK_FORMAT_R8G8B8A8_UINT;
    info.tiling        = VK_IMAGE_TILING_OPTIMAL;
    info.image_usage   = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.aspectMask    = VK_IMAGE_ASPECT_COLOR_BIT;
    info.memory_usage  = VMA_MEMORY_USAGE_GPU_ONLY;

    if (nuvk_image_create(&geometry->image, context, memory_manager, &info) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create geometry image.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_image_geometry_destroy(
    nuvk_sdf_image_geometry_t *geometry,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_image_destroy(&geometry->image, context, memory_manager);
    
    return NU_SUCCESS;
}