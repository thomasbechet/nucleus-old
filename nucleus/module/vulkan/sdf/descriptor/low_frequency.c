#include <nucleus/module/vulkan/sdf/descriptor/low_frequency.h>

nu_result_t nuvk_sdf_descriptor_low_frequency_create(
    nuvk_sdf_descriptor_low_frequency_t *descriptor,
    const nuvk_context_t *context,
    const nuvk_sdf_buffer_environment_t *environment_buffer,
    const nuvk_sdf_buffer_instances_t *instances_buffer,
    VkDescriptorPool pool
)
{
    /* create layout */
    VkDescriptorSetLayoutBinding bindings[3];
    memset(bindings, 0, sizeof(VkDescriptorSetLayoutBinding) * 3);

    bindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    bindings[0].binding         = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    bindings[1].binding         = 1;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[2].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    bindings[2].binding         = 2;
    bindings[2].descriptorCount = 1;
    bindings[2].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
    layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 3;
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
    VkDescriptorBufferInfo buffer_info[3];
    memset(buffer_info, 0, sizeof(VkDescriptorBufferInfo) * 3);

    buffer_info[0].buffer = environment_buffer->buffer.buffer;
    buffer_info[0].offset = 0;
    buffer_info[0].range  = environment_buffer->uniform_buffer_range;

    buffer_info[1].buffer = instances_buffer->index_buffer.buffer;
    buffer_info[1].offset = 0;
    buffer_info[1].range  = instances_buffer->index_uniform_buffer_range;

    buffer_info[2].buffer = instances_buffer->instance_buffer.buffer;
    buffer_info[2].offset = 0;
    buffer_info[2].range  = instances_buffer->instance_uniform_buffer_range;

    VkWriteDescriptorSet writes[3];
    memset(writes, 0, sizeof(VkWriteDescriptorSet) * 3);

    writes[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstBinding      = 0;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writes[0].dstSet          = descriptor->descriptor;
    writes[0].pBufferInfo     = &buffer_info[0];

    writes[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstBinding      = 1;
    writes[1].descriptorCount = 1;
    writes[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writes[1].dstSet          = descriptor->descriptor;
    writes[1].pBufferInfo     = &buffer_info[1];

    writes[2].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[2].dstBinding      = 2;
    writes[2].descriptorCount = 1;
    writes[2].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writes[2].dstSet          = descriptor->descriptor;
    writes[2].pBufferInfo     = &buffer_info[2];

    vkUpdateDescriptorSets(context->device, 3, writes, 0, NULL);

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