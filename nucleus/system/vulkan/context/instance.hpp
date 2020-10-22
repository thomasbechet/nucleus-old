#pragma once

#include "../utility/internalptr.hpp"
#include "../utility/glfwinterface.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class Instance
    {
    public:
        static inline constexpr std::string_view Section = "INSTANCE";

        Instance(GLFWInterface &interface, bool enableValidationLayers = true);

        vk::Instance &getInstance();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}