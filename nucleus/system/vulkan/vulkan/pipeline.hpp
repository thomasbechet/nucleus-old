#pragma once

#include "../utility/internalptr.hpp"
#include "device.hpp"
#include "memoryallocator.hpp"

#include <vulkan/vulkan.h>
#include <nucleus/nucleus.h>

#include <array>
#include <string>

namespace nuvk
{   
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
            const MemoryAllocator &memoryAllocator,
            VkRenderPass renderPass,
            VkExtent2D extent,
            uint32_t frameResourceCount
        );

        VkPipeline getPipeline() const;
        void updateUBO(const UniformBufferObject &ubo, uint32_t frameIndex);
        VkDescriptorSet getDescriptorSet(uint32_t frameIndex) const;
        VkPipelineLayout getPipelineLayout() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}