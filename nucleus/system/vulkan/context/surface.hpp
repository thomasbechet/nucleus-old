#pragma once

#include "../utility/internalptr.hpp"
#include "../utility/glfwinterface.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class Surface
    {
    public:
        static inline constexpr std::string_view Section = "SURFACE";

        Surface(
            vk::UniqueInstance &instance,
            GLFWInterface &glfwInterface
        );
        
        VkSurfaceKHR getSurface();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}