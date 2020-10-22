#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class CommandPool
    {
    public:
        CommandPool(
            const vk::Device &device,
            uint32_t queueIndex
        );

        vk::UniqueCommandBuffer beginCommandBuffer(const vk::Device &device) const;
        void endCommandBuffer(const vk::CommandBuffer &commandBuffer, const vk::Queue &queue) const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}