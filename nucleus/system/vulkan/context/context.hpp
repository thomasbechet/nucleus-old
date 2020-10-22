#pragma once

#include "../utility/internalptr.hpp"
#include "../utility/glfwinterface.hpp"

#include <vulkan/vulkan.hpp>

#include <string>

namespace nuvk
{
    class Context
    {
    public:
        static inline constexpr std::string_view Section = "CONTEXT";
        
        Context();

        static std::vector<const char*> GetRequiredExtensions(GLFWInterface &glfwInterface, bool useValidationLayers);
        static std::vector<const char*> GetRequiredValidationLayers();
        static std::vector<const char*> GetRequiredDeviceExtensions();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}