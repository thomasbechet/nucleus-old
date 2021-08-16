#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>

#include <nucleus/module/vulkan/sdf/descriptor/pool.h>

nu_result_t nuvk_sdf_descriptors_initialize(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context,
    const nuvk_sdf_buffers_t *buffers,
    const nuvk_sdf_images_t *images
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_descriptor_pool_create(&descriptors->pool, context, 2);
    result &= nuvk_sdf_descriptor_low_frequency_create(&descriptors->low_frequency, context, 
        &buffers->environment, &buffers->instances, descriptors->pool);
    result &= nuvk_sdf_descriptor_postprocess_create(&descriptors->postprocess, context,
        &images->geometry, descriptors->pool);

    return result;
}
nu_result_t nuvk_sdf_descriptors_terminate(
    nuvk_sdf_descriptors_t *descriptors,
    const nuvk_context_t *context
)
{
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
    return nuvk_sdf_descriptor_postprocess_update_swapchain(&descriptors->postprocess, context, &images->geometry);
}