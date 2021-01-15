#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.h>

namespace nuvk
{
    class Image
    {
    public:
        Image(

        );

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        uint32_t getMipLevels() const;
        vk::Format getFormat() const;
        const vk::Image &getImage() const;

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
}