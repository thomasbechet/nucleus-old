#pragma once

#include "../utility/internalptr.hpp"
#include "device.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class CommandPool
    {
    public:
        static inline constexpr std::string_view Section = "COMMANDPOOL";

        CommandPool(
            const Device &device,
            uint32_t queueIndex
        );

        vk::UniqueCommandBuffer beginCommandBuffer() const;
        void endCommandBuffer(const vk::CommandBuffer &commandBuffer, const vk::Queue &queue) const;

        std::vector<vk::UniqueCommandBuffer> createCommandBuffers(const vk::Device &device, uint32_t count) const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}