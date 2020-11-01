#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>
#include <nucleus/nucleus.h>

#include <array>

namespace nuvk
{   
    struct Vertex
    {
        nu_vec2_t position;
        nu_vec3_t color;

        static vk::VertexInputBindingDescription GetBindingDescription() {
            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = vk::VertexInputRate::eVertex;
            return bindingDescription;
        }
        static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions() {
            std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[0].offset = offsetof(Vertex, position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    class Pipeline
    {
    public:
        static inline constexpr std::string_view Section = "PIPELINE";

        Pipeline(
            const vk::Device &device,
            const vk::RenderPass &renderPass,
            vk::Extent2D extent
        );

        const vk::Pipeline &getPipeline() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}