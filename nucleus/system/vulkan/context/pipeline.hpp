#pragma once

#include "../utility/internalptr.hpp"
#include "rendercontext.hpp"
#include "device.hpp"

#include <vulkan/vulkan.h>
#include <nucleus/nucleus.h>

#include <array>
#include <string>

namespace nuvk
{   
    struct Vertex
    {
        nu_vec2_t position;
        nu_vec3_t color;

        static VkVertexInputBindingDescription GetBindingDescription() {
            VkVertexInputBindingDescription bindingDescription;
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }
        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

            attributeDescriptions[0].binding  = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset   = offsetof(Vertex, position);

            attributeDescriptions[1].binding  = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset   = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    struct UniformBufferObject
    {
        nu_mat4_t model;
        nu_mat4_t view;
        nu_mat4_t projection;

        static VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() {
            VkDescriptorSetLayoutBinding uboLayoutBinding;

            uboLayoutBinding.binding         = 0;
            uboLayoutBinding.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboLayoutBinding.descriptorCount = 1;
            uboLayoutBinding.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
            
            return uboLayoutBinding;
        }
    };

    class Pipeline
    {
    public:
        static inline constexpr std::string_view Section = "PIPELINE";

        Pipeline(
            const Device &device,
            const RenderContext &renderContext
        );

        VkPipeline getPipeline() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}