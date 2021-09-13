#include <nucleus/module/vulkan/sdf/descriptor/light.h>

static void nuvk_sdf_descriptor_light_write_descriptor(
    nuvk_sdf_descriptor_light_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *geometry_image,
    const nuvk_sdf_image_light_t *light_image
)
{
    VkDescriptorImageInfo image_info[3];
    memset(&image_info, 0, sizeof(VkDescriptorImageInfo) * 3);

    image_info[0].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    image_info[0].imageView   = geometry_image->image.image_view;

    image_info[1].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    image_info[1].imageView   = light_image->image.image_view;

    VkWriteDescriptorSet writes[2];
    memset(writes, 0, sizeof(VkWriteDescriptorSet) * 2);

    writes[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstBinding      = 0;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    writes[0].dstSet          = descriptor->descriptor;
    writes[0].pImageInfo      = &image_info[0];

    writes[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstBinding      = 1;
    writes[1].descriptorCount = 1;
    writes[1].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    writes[1].dstSet          = descriptor->descriptor;
    writes[1].pImageInfo      = &image_info[1];

    vkUpdateDescriptorSets(context->device, 2, writes, 0, NULL);
}

nu_result_t nuvk_sdf_descriptor_light_create(
    nuvk_sdf_descriptor_light_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *geometry_image,
    const nuvk_sdf_image_light_t *light_image,
    VkDescriptorPool pool
)
{
    /* create layout */
    VkDescriptorSetLayoutBinding bindings[2];
    memset(bindings, 0, sizeof(VkDescriptorSetLayoutBinding) * 2);

    bindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[0].binding         = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[1].binding         = 1;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
    layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 2;
    layout_info.pBindings    = bindings;

    VkResult result = vkCreateDescriptorSetLayout(context->device, &layout_info, &context->allocator, &descriptor->layout);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create light descriptor set layout.");

    /* create descriptor set */
    VkDescriptorSetAllocateInfo allocate_info;
    memset(&allocate_info, 0, sizeof(VkDescriptorSetAllocateInfo));
    allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocate_info.descriptorPool     = pool;
    allocate_info.descriptorSetCount = 1;
    allocate_info.pSetLayouts        = &descriptor->layout;

    result = vkAllocateDescriptorSets(context->device, &allocate_info, &descriptor->descriptor);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to allocate light descriptor set.");

    /* write descriptor set */
    nuvk_sdf_descriptor_light_write_descriptor(descriptor, context, geometry_image, light_image);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_descriptor_light_destroy(
    nuvk_sdf_descriptor_light_t *descriptor,
    const nuvk_context_t *context
)
{
    vkDestroyDescriptorSetLayout(context->device, descriptor->layout, &context->allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_descriptor_light_update_images(
    nuvk_sdf_descriptor_light_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_image_geometry_t *geometry_image,
    const nuvk_sdf_image_light_t *light_image
)
{
    nuvk_sdf_descriptor_light_write_descriptor(descriptor, context, geometry_image, light_image);

    return NU_SUCCESS;
}