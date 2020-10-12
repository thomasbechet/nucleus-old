#pragma once

#include <memory>

namespace nuvk
{
    namespace internal
    {
        template<class T>
        void deleter(T *c)
        {
            delete c;
        }
    }

    template<class T, class Deleter = void (*)(T*)>
    using InternalPtr = std::unique_ptr<T, Deleter>;

    template<class T, class... Args>
    inline InternalPtr<T> MakeInternalPtr(Args&&... args)
    {
        return InternalPtr<T>(new T(std::forward<Args>(args)...), &internal::deleter<T>);
    }
}