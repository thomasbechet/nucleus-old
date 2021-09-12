#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>

#include <nucleus/module/vulkan/sdf/descriptor/pool.h>

nu_result_t nuvk_sdf_descriptors_initialize(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context,
    const nuvk_sdf_buffers_t *buffers,
    const nuvk_sdf_images_t *images
)
{
    nu_result_t result;

    result = nuvk_sdf_descriptor_pool_create(&descriptors->pool, context, 2);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create descriptor pool.");
    result = nuvk_sdf_descriptor_low_frequency_create(&descriptors->low_frequency, context, 
        &buffers->environment, &buffers->instances, descriptors->pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create low frequency descriptor.");
    result = nuvk_sdf_descriptor_light_create(&descriptors->light, context,
        &images->geometry, &images->light, descriptors->pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create light descriptor.");
    result = nuvk_sdf_descriptor_postprocess_create(&descriptors->postprocess, context,
        &images->light, descriptors->pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create postprocess descriptor.");

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_descriptors_terminate(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context
)
{
    nuvk_sdf_descriptor_light_destroy(&descriptors->light, context);
    nuvk_sdf_descriptor_postprocess_destroy(&descriptors->postprocess, context);
    nuvk_sdf_descriptor_low_frequency_destroy(&descriptors->low_frequency, context);
    vkDestroyDescriptorPool(context->device, descriptors->pool, &context->allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_descriptor_update_swapchain(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context,
    const nuvk_sdf_images_t *images
)
{
    nu_result_t result;

    result = nuvk_sdf_descriptor_light_update_swapchain(&descriptors->light, context, &images->geometry, &images->light);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update light descriptor.");
    result = nuvk_sdf_descriptor_postprocess_update_swapchain(&descriptors->postprocess, context, &images->light);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update postprocess descriptor.");

    return NU_SUCCESS;
}