#pragma once

#include "../utility/internalptr.hpp"
#include "device.hpp"
#include "swapchain.hpp"

#include <vulkan/vulkan.h>

namespace nuvk
{
    class RenderPass
    {
    public:
        static constexpr std::string_view Section = "RENDERPASS";

        RenderPass(
            const Device &device,
            const Swapchain &swapchain
        );

        VkRenderPass getRenderPass() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}