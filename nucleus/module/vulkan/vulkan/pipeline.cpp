#include "pipeline.hpp"

#include "../engine/engine.hpp"
#include "../utility/shaderhelper.hpp"
#include "../asset/mesh.hpp"
#include "buffer.hpp"

#include <cstring>

using namespace nuvk;

namespace 
{
    static VkDescriptorSetLayout CreateDescriptorSetLayout(
        VkDevice device
    )
    {
        auto descriptorSetLayoutBinding = UniformBufferObject::GetDescriptorSetLayoutBinding();

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings    = &descriptorSetLayoutBinding;

        VkDescriptorSetLayout descriptorSetLayout;
        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            Engine::Interrupt(Pipeline::Section, "Failed to create descriptor set layout.");
        }
        return descriptorSetLayout;
    }

    static VkPipelineLayout CreatePipelineLayout(
        VkDevice device,
        VkDescriptorSetLayout descriptorSetLayout
    )
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

        VkPipelineLayout pipelineLayout;
        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            Engine::Interrupt(Pipeline::Section, "Failed to create pipeline layout.");
        }
        return pipelineLayout;
    }

    static VkPipeline CreatePipeline(
        VkDevice device,
        VkPipelineLayout pipelineLayout,
        VkRenderPass renderPass,
        VkExtent2D extent
    )
    {
        VkShaderModule vertShaderModule = ShaderHelper::CreateShaderModule(device, "engine/shader/spirv/shader.vert.spv");
        VkShaderModule fragShaderModule = ShaderHelper::CreateShaderModule(device, "engine/shader/spirv/shader.frag.spv");

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};
        shaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertShaderModule;
        shaderStages[0].pName  = "main";
        shaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].pName  = "main";

        auto bindingDescription = Mesh::GetVertexInputBindingDescription();
        auto attributeDescriptions = Mesh::GetVertexAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount   = 1;
        vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport;
        viewport.x        = 0.0f;
        viewport.y        = (float)extent.height;
        viewport.width    = (float)extent.width;
        viewport.height   = -(float)extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor;
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent   = extent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports    = &viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable        = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth               = 1.0f;
        //rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
        rasterizer.cullMode                = VK_CULL_MODE_NONE;
        rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable         = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable  = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | 
                                              VK_COLOR_COMPONENT_G_BIT | 
                                              VK_COLOR_COMPONENT_B_BIT | 
                                              VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable    = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable     = VK_FALSE;
        colorBlending.logicOp           = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount   = 1;
        colorBlending.pAttachments      = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount          = 2;
        pipelineInfo.pStages             = shaderStages.data();
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState   = &multisampling;
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.layout              = pipelineLayout;
        pipelineInfo.renderPass          = renderPass;
        pipelineInfo.subpass             = 0;
        pipelineInfo.basePipelineHandle  = nullptr;

        VkPipeline pipeline;
        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
            Engine::Interrupt(Pipeline::Section, "Failed to create graphics pipeline.");
        }

        vkDestroyShaderModule(device, vertShaderModule, nullptr);
        vkDestroyShaderModule(device, fragShaderModule, nullptr);

        return pipeline;
    }

    static std::vector<std::pair<UniformBufferObject*, std::unique_ptr<Buffer>>> CreateUniformBuffers(
        const MemoryAllocator &memoryAllocator,
        uint32_t count
    )
    {
        std::vector<std::pair<UniformBufferObject*, std::unique_ptr<Buffer>>> uniformBuffers;
        uniformBuffers.resize(count);

        for (auto &ubo : uniformBuffers) {
            ubo.second = std::make_unique<Buffer>(
                memoryAllocator,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VMA_MEMORY_USAGE_CPU_TO_GPU,
                sizeof(UniformBufferObject),
                nullptr
            );
            ubo.first = static_cast<UniformBufferObject*>(ubo.second->map());
        }

        return uniformBuffers;
    }

    static VkDescriptorPool CreateDescriptorPool(VkDevice device, uint32_t size)
    {
        VkDescriptorPoolSize poolSize{};
        poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = size;

        VkDescriptorPoolCreateInfo info{};
        info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.poolSizeCount = 1;
        info.pPoolSizes    = &poolSize;
        info.maxSets       = size;

        VkDescriptorPool pool;
        if (vkCreateDescriptorPool(device, &info, nullptr, &pool) != VK_SUCCESS) {
            Engine::Interrupt(Pipeline::Section, "Failed to create descriptor pool.");
        }

        return pool;
    }

    static std::vector<VkDescriptorSet> CreateDescriptorSets(
        VkDevice device, 
        VkDescriptorSetLayout layout, 
        VkDescriptorPool pool, 
        uint32_t count 
    )
    {
        std::vector<VkDescriptorSetLayout> layouts(count, layout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool     = pool;
        allocInfo.descriptorSetCount = count;
        allocInfo.pSetLayouts        = layouts.data();
        
        std::vector<VkDescriptorSet> descriptorSets;
        descriptorSets.resize(count);
        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            Engine::Interrupt(Pipeline::Section, "Failed to create descriptor sets.");
        }
        
        return descriptorSets;
    }
}

struct Pipeline::Internal
{
    VkDevice device;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;

    std::vector<std::pair<UniformBufferObject*, std::unique_ptr<Buffer>>> uniformBuffers;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    Internal(
        const Device &device,
        const MemoryAllocator &memoryAllocator,
        VkRenderPass renderPass,
        VkExtent2D extent,
        uint32_t frameResourceCount
    ) : device(device.getDevice())
    {
        descriptorSetLayout = ::CreateDescriptorSetLayout(device.getDevice());
        pipelineLayout = ::CreatePipelineLayout(device.getDevice(), descriptorSetLayout);
        pipeline = ::CreatePipeline(device.getDevice(), pipelineLayout, renderPass, extent);

        uniformBuffers = ::CreateUniformBuffers(memoryAllocator, frameResourceCount);
        descriptorPool = ::CreateDescriptorPool(device.getDevice(), frameResourceCount);
        descriptorSets = ::CreateDescriptorSets(device.getDevice(), descriptorSetLayout, descriptorPool, frameResourceCount);
    
        // Write descriptor sets
        for (uint32_t i = 0; i < frameResourceCount; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers.at(i).second->getBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range  = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet           = descriptorSets.at(i);
            descriptorWrite.dstBinding       = 0;
            descriptorWrite.dstArrayElement  = 0;
            descriptorWrite.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount  = 1;
            descriptorWrite.pBufferInfo      = &bufferInfo;
            descriptorWrite.pImageInfo       = nullptr;
            descriptorWrite.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(device.getDevice(), 1, &descriptorWrite, 0, nullptr);
        }
    }
    ~Internal()
    {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);

        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    }
};

Pipeline::Pipeline(
    const Device &device,
    const MemoryAllocator &memoryAllocator,
    VkRenderPass renderPass,
    VkExtent2D extent,
    uint32_t frameResourceCount
) : internal(MakeInternalPtr<Internal>(device, memoryAllocator, renderPass, extent, frameResourceCount)) {}

VkPipeline Pipeline::getPipeline() const
{
    return internal->pipeline;
}
void Pipeline::updateUBO(const UniformBufferObject &ubo, uint32_t frameIndex)
{
    std::memcpy(internal->uniformBuffers.at(frameIndex).first, &ubo, sizeof(UniformBufferObject));
}
VkDescriptorSet Pipeline::getDescriptorSet(uint32_t frameIndex) const
{
    return internal->descriptorSets.at(frameIndex);
}
VkPipelineLayout Pipeline::getPipelineLayout() const
{
    return internal->pipelineLayout;
}