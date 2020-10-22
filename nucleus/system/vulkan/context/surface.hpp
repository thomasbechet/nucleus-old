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
            const vk::Instance &instance,
            GLFWInterface &glfwInterface
        );
        
        VkSurfaceKHR getSurface();
        vk::Format getFormat();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}