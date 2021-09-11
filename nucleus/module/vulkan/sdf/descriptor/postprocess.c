#include <nucleus/module/vulkan/sdf/descriptor/postprocess.h>

static void nuvk_sdf_descriptor_postprocess_write_descriptor(
    nuvk_sdf_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_light_t *image
)
{
    VkDescriptorImageInfo image_info;
    memset(&image_info, 0, sizeof(VkDescriptorImageInfo));

    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    image_info.imageView   = image->image.image_view;
    image_info.sampler     = descriptor->sampler;

    VkWriteDescriptorSet writes[1];
    memset(writes, 0, sizeof(VkWriteDescriptorSet) * 1);

    writes[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstBinding      = 0;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[0].dstSet          = descriptor->descriptor;
    writes[0].pImageInfo      = &image_info;

    vkUpdateDescriptorSets(context->device, 1, writes, 0, NULL);
}

nu_result_t nuvk_sdf_descriptor_postprocess_create(
    nuvk_sdf_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_light_t *image,
    VkDescriptorPool pool
)
{
    /* create sampler */
    VkSamplerCreateInfo sampler_info;
    memset(&sampler_info, 0, sizeof(VkSamplerCreateInfo));
    sampler_info.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter               = VK_FILTER_LINEAR;
    sampler_info.minFilter               = VK_FILTER_LINEAR;
    sampler_info.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_info.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_info.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_info.anisotropyEnable        = VK_FALSE;
    sampler_info.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable           = VK_FALSE;
    sampler_info.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias              = 0.0f;
    sampler_info.minLod                  = 0.0f;
    sampler_info.maxLod                  = 0.0f;

    if (vkCreateSampler(context->device, &sampler_info, &context->allocator, &descriptor->sampler) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess sampler.\n");
        return NU_FAILURE;
    }

    /* create layout */
    VkDescriptorSetLayoutBinding bindings[1];
    memset(bindings, 0, sizeof(VkDescriptorSetLayoutBinding) * 1);

    bindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[0].binding         = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
    layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings    = bindings;

    if (vkCreateDescriptorSetLayout(context->device, &layout_info, &context->allocator, &descriptor->layout) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess descriptor set layout.\n");
        return NU_FAILURE;
    }

    /* create descriptor set */
    VkDescriptorSetAllocateInfo allocate_info;
    memset(&allocate_info, 0, sizeof(VkDescriptorSetAllocateInfo));
    allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocate_info.descriptorPool     = pool;
    allocate_info.descriptorSetCount = 1;
    allocate_info.pSetLayouts        = &descriptor->layout;

    if (vkAllocateDescriptorSets(context->device, &allocate_info, &descriptor->descriptor) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to allocate postprocess descriptor set.\n");
        return NU_FAILURE;
    }

    /* write descriptor set */
    nuvk_sdf_descriptor_postprocess_write_descriptor(descriptor, context, image);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_descriptor_postprocess_destroy(
    nuvk_sdf_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context
)
{
    vkDestroyDescriptorSetLayout(context->device, descriptor->layout, &context->allocator);
    vkDestroySampler(context->device, descriptor->sampler, &context->allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_descriptor_postprocess_update_swapchain(
    nuvk_sdf_descriptor_postprocess_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_light_t *image
)
{
    nuvk_sdf_descriptor_postprocess_write_descriptor(descriptor, context, image);

    return NU_SUCCESS;
}