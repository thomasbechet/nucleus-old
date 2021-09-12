#include <nucleus/module/vulkan/sdf/image/images.h>

nu_result_t nuvk_sdf_images_initialize(
    nuvk_sdf_images_t *images,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
)
{
    nu_result_t result;

    result = nuvk_sdf_image_geometry_create(&images->geometry, context, memory_manager, swapchain);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry image.");
    result = nuvk_sdf_image_light_create(&images->light, context, memory_manager, swapchain);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light image.");


    return result;
}
nu_result_t nuvk_sdf_images_terminate(
    nuvk_sdf_images_t *images,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_sdf_image_light_destroy(&images->light, context, memory_manager);
    nuvk_sdf_image_geometry_destroy(&images->geometry, context, memory_manager);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_images_update_swapchain(
    nuvk_sdf_images_t *images,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
)
{
    nuvk_sdf_image_light_destroy(&images->light, context, memory_manager);
    nuvk_sdf_image_geometry_destroy(&images->geometry, context, memory_manager);

    nu_result_t result;

    result = nuvk_sdf_image_geometry_create(&images->geometry, context, memory_manager, swapchain);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry image.");
    result = nuvk_sdf_image_light_create(&images->light, context, memory_manager, swapchain);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light image.");

    return result;
}