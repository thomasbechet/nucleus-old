#pragma once

#include "../utility/internalptr.hpp"
#include "instance.hpp"
#include "windowinterface.hpp"

#include <vulkan/vulkan.h>

namespace nuvk
{
    class Surface
    {
    public:
        static inline constexpr std::string_view Section = "SURFACE";

        Surface(
            const Instance &instance,
            const WindowInterface &interface
        );
        
        VkSurfaceKHR getSurface() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}