#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class Pipeline
    {
    public:
        Pipeline();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}