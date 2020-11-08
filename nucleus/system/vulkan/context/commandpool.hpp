#pragma once

#include "../utility/internalptr.hpp"
#include "device.hpp"

#include <vulkan/vulkan.h>

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

        VkCommandBuffer beginCommandBuffer() const;
        void endCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue) const;

        std::vector<VkCommandBuffer> createCommandBuffers(uint32_t count) const;
        void destroyCommandBuffer(VkCommandBuffer commandBuffer) const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}