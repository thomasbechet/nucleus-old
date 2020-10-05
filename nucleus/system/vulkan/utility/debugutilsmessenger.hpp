#pragma once

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class DebugUtilsMessenger
    {
    public:
        DebugUtilsMessenger(vk::UniqueInstance &instance);
        ~DebugUtilsMessenger();

    private:
        VkDebugUtilsMessengerEXT m_callback;
        vk::UniqueInstance &m_instance;
    };
}