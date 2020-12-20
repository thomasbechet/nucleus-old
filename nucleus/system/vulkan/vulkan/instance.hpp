#pragma once

#include "../utility/internalptr.hpp"
#include "windowinterface.hpp"

#include <vulkan/vulkan.h>

namespace nuvk
{
    class Instance
    {
    public:
        static inline constexpr std::string_view Section = "INSTANCE";

        Instance(
            const WindowInterface &interface, 
            bool enableValidationLayers = true
        );

        VkInstance getInstance() const;

        static std::vector<const char*> GetRequiredExtensions(const WindowInterface &interface, bool useValidationLayers);
        static uint32_t GetVulkanAPIVersion();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}