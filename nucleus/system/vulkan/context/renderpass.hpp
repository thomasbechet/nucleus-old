#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class RenderPass
    {
    public:
        static constexpr std::string_view Section = "RENDERPASS";

        RenderPass(
            const vk::Device &device,
            vk::Format swapChainFormat
        );

        vk::RenderPass &getRenderPass(); 

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}