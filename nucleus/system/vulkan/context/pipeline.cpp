#include "pipeline.hpp"

#include "../engine/engine.hpp"
#include "../utility/shaderhelper.hpp"

using namespace nuvk;

namespace 
{
    static vk::UniqueDescriptorSetLayout CreateDescriptorSetLayout(
        const vk::Device &device
    )
    {
        auto descriptorSetLayoutBinding = UniformBufferObject::GetDescriptorSetLayoutBinding();

        vk::DescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &descriptorSetLayoutBinding;

        try {
            return device.createDescriptorSetLayoutUnique(layoutInfo);
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create descriptor set layout.");
        }

        throw std::runtime_error("Unknown error.");
    }

    static vk::UniquePipelineLayout CreatePipelineLayout(
        const vk::Device &device,
        const vk::DescriptorSetLayout &descriptorSetLayout
    )
    {
        vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        pipelineLayoutInfo.setLayoutCount = 0;
        //pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

        try {
            return device.createPipelineLayoutUnique(pipelineLayoutInfo);
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create pipeline layout.");
        }

        throw std::runtime_error("Unknown error.");
    }

    static vk::UniquePipeline CreatePipeline(
        const vk::Device &device,
        const vk::PipelineLayout &pipelineLayout,
        const vk::RenderPass &renderPass,
        vk::Extent2D extent
    )
    {
        auto vertShaderModule = ShaderHelper::CreateShaderModule(device, "engine/shader/spirv/shader.vert.spv");
        auto fragShaderModule = ShaderHelper::CreateShaderModule(device, "engine/shader/spirv/shader.frag.spv");

        vk::PipelineShaderStageCreateInfo shaderStages[] = {
            {
                vk::PipelineShaderStageCreateFlags(),
                vk::ShaderStageFlagBits::eVertex,
                *vertShaderModule,
                "main"
            },
            {
                vk::PipelineShaderStageCreateFlagBits(),
                vk::ShaderStageFlagBits::eFragment,
                *fragShaderModule,
                "main"
            }
        };

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
        auto bindingDescription = Vertex::GetBindingDescription();
        auto attributeDescriptions = Vertex::GetAttributeDescriptions();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)extent.width;
        viewport.height = (float)extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vk::Rect2D scissor = {};
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent = extent;

        vk::PipelineViewportStateCreateInfo viewportState = {};
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        vk::PipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = vk::PolygonMode::eFill;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace = vk::FrontFace::eClockwise;
        rasterizer.depthBiasEnable = VK_FALSE;

        vk::PipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_FALSE;

        vk::PipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = nullptr;

        try {
            return device.createGraphicsPipelineUnique(nullptr, pipelineInfo).value;
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create graphics pipeline.");
        }

        throw std::runtime_error("Unknown error.");
    }
}

struct Pipeline::Internal
{
    vk::UniqueDescriptorSetLayout descriptorSetLayout;
    vk::UniquePipelineLayout pipelineLayout;
    vk::UniquePipeline pipeline;

    Internal(
        const vk::Device &device,
        const vk::RenderPass &renderPass,
        vk::Extent2D extent
    )
    {
        descriptorSetLayout = ::CreateDescriptorSetLayout(device);
        pipelineLayout = ::CreatePipelineLayout(device, *descriptorSetLayout);
        pipeline = ::CreatePipeline(device, *pipelineLayout, renderPass, extent);
    }
    ~Internal()
    {

    }
};

Pipeline::Pipeline(
    const vk::Device &device,
    const vk::RenderPass &renderPass,
    vk::Extent2D extent
) : internal(MakeInternalPtr<Internal>(device, renderPass, extent)) {}

const vk::Pipeline &Pipeline::getPipeline() const
{
    return *internal->pipeline;
}

