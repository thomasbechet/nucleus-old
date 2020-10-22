#pragma once

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class DebugUtilsMessenger
    {
    public:
        DebugUtilsMessenger(const vk::Instance &instance);
        ~DebugUtilsMessenger();

    private:
        VkDebugUtilsMessengerEXT m_callback;
        const vk::Instance &m_instance;
    };
}