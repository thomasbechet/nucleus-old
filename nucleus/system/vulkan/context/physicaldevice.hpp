#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class PhysicalDevice
    {
    public:
        inline constexpr auto Section = "PHYSICALDEVICE";
        static IsSuitable(vk::PhysicalDevice &device, vk::SurfaceKHR &surface);

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}