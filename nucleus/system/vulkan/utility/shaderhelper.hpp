#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace nuvk
{
    class ShaderHelper
    {
    public:
        static VkShaderModule CreateShaderModule(VkDevice device, const std::string &filename);
    };
}