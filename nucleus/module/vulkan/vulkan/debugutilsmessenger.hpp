#pragma once

#include "../utility/internalptr.hpp"
#include "instance.hpp"

namespace nuvk
{
    class DebugUtilsMessenger
    {
    public:
        static inline constexpr std::string_view Section = "DEBUG";

        DebugUtilsMessenger(
            const Instance &instance
        );
        
    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}