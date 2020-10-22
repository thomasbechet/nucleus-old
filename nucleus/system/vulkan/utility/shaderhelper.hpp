#pragma once

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class ShaderHelper
    {
    public:
        static vk::UniqueShaderModule CreateShaderModule(const vk::Device &device, const std::string &filename);
    };
}