#pragma once

#include "../utility/internalptr.hpp"
#include "../context/context.hpp"

namespace nuvk
{
    class RenderContext
    {
    public:
        RenderContext(Context &context);

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}