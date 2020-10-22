#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class Pipeline
    {
    public:
        static inline constexpr std::string_view Section = "PIPELINE";

        Pipeline(
            const vk::Device &device,
            const vk::RenderPass &renderPass,
            vk::Extent2D extent
        );

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}