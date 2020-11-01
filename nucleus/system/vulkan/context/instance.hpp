#pragma once

#include "../utility/internalptr.hpp"
#include "windowinterface.hpp"

#include <vulkan/vulkan.hpp>

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

        const vk::Instance &getInstance() const;

        static std::vector<const char*> GetRequiredExtensions(const WindowInterface &interface, bool useValidationLayers);

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}