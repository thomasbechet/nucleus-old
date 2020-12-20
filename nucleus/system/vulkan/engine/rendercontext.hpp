#pragma once

#include "../utility/internalptr.hpp"
#include "../vulkan/commandpool.hpp"
#include "../vulkan/device.hpp"
#include "../vulkan/physicaldevice.hpp"
#include "../vulkan/surface.hpp"

#include <vulkan/vulkan.h>

namespace nuvk
{
    class RenderContext
    {
    public:
        static inline constexpr std::string_view Section = "RENDERCONTEXT";

        RenderContext(
            const Device &device,
            const PhysicalDevice &physicalDevice,
            const Surface &surface,
            const CommandPool &commandPool,
            uint32_t width, uint32_t height
        );

        bool beginRender();
        bool endRender();

        VkViewport getViewport() const;
        VkRect2D getScissor() const;
        VkExtent2D getExtent() const;
        VkRenderPass getRenderPass() const;

        VkCommandBuffer getActiveCommandBuffer() const;
        uint32_t getActiveFrameResourceIndex() const;
        uint32_t getFrameResourceCount() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}