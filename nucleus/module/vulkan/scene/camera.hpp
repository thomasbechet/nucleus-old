#pragma once

#include "../utility/internalptr.hpp"

namespace nuvk
{
    class Camera
    {
    public:
        

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}