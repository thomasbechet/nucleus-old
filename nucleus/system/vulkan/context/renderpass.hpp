#pragma once

#include "../utility/internalptr.hpp"
#include "device.hpp"
#include "swapchain.hpp"

#include <vulkan/vulkan.hpp>

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

        const vk::RenderPass &getRenderPass() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}