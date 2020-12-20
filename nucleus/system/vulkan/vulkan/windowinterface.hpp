#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace nuvk
{
    class WindowInterface
    {
    public:
        static inline constexpr std::string_view Section = "WINDOWINTERFACE";

        WindowInterface();

        std::vector<const char*> getRequiredInstanceExtensions() const;
        VkSurfaceKHR createWindowSurface(VkInstance instance) const;
    
    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}