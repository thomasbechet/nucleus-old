#include <nucleus/module/vulkan/image/light.h>

nu_result_t nuvk_image_light_create(
    nuvk_image_light_t *light,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nu_vec2u_t resolution
)
{
    nuvk_image_info_t info;
    memset(&info, 0, sizeof(nuvk_image_info_t));
    info.extent.width  = resolution[0];
    info.extent.height = resolution[1];
    info.format        = VK_FORMAT_R16G16B16A16_SFLOAT;
    info.tiling        = VK_IMAGE_TILING_OPTIMAL;
    info.image_usage   = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
    info.aspectMask    = VK_IMAGE_ASPECT_COLOR_BIT;
    info.memory_usage  = VMA_MEMORY_USAGE_GPU_ONLY;

    nu_result_t result = nuvk_image_create(&light->image, context, memory_manager, &info);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry image.");

    return NU_SUCCESS;
}
nu_result_t nuvk_image_light_destroy(
    nuvk_image_light_t *light,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_image_destroy(&light->image, context, memory_manager);
    
    return NU_SUCCESS;
}