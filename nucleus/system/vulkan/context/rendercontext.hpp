#pragma once

#include "../utility/internalptr.hpp"
#include "commandpool.hpp"
#include "device.hpp"
#include "physicaldevice.hpp"
#include "surface.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class RenderContext
    {
    public:
        RenderContext(
            const Device &device,
            const PhysicalDevice &physicalDevice,
            const Surface &surface,
            const CommandPool &commandPool,
            uint32_t width, uint32_t height
        );

        bool beginRender();
        bool endRender();

        const vk::Viewport &getViewport() const;
        const vk::Rect2D &getScissor() const;
        const vk::RenderPass &getRenderPass() const;
        const vk::CommandBuffer &getActiveCommandBuffer() const;
        vk::Extent2D getSwapchainExtent() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}