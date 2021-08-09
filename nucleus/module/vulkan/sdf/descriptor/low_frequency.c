#include <nucleus/module/vulkan/sdf/descriptor/low_frequency.h>

nu_result_t nuvk_sdf_descriptor_low_frequency_create(
    nuvk_sdf_descriptor_low_frequency_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_buffer_low_frequency_t *low_frequency_buffer,
    VkDescriptorPool pool
)
{
    /* create layout */
    VkDescriptorSetLayoutBinding bindings[1];
    memset(bindings, 0, sizeof(VkDescriptorSetLayoutBinding) * 1);
    bindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
    layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings    = bindings;

    if (vkCreateDescriptorSetLayout(context->device, &layout_info, &context->allocator, &descriptor->layout) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create low frequency descriptor set layout.\n");
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
        nu_error(NUVK_LOGGER_NAME"Failed to allocate low frequency descriptor set.\n");
        return NU_FAILURE;
    }

    /* write descriptor set */
    VkDescriptorBufferInfo buffer_info;
    buffer_info.buffer = low_frequency_buffer->buffer.buffer;
    buffer_info.offset = 0;
    buffer_info.range  = low_frequency_buffer->uniform_buffer_size;

    VkWriteDescriptorSet write;
    memset(&write, 0, sizeof(VkWriteDescriptorSet));
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding      = 0;
    write.descriptorCount = 1;
    write.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    write.dstSet          = descriptor->descriptor;
    write.pBufferInfo     = &buffer_info;

    vkUpdateDescriptorSets(context->device, 1, &write, 0, NULL);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_descriptor_low_frequency_destroy(
    nuvk_sdf_descriptor_low_frequency_t *descriptor,
    const nuvk_context_t *context
)
{
    vkDestroyDescriptorSetLayout(context->device, descriptor->layout, &context->allocator);

    return NU_SUCCESS;
}