#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

namespace nuvk
{
    class Instance
    {
    public:
        Instance(bool enableValidationLayers = true);

        vk::UniqueInstance &getInstance();

        static bool CheckValidationLayerSupport(const std::vector<const char*> &instanceLayers);

    private:
        struct Instance;
        InternalPtr<Instance> instance;
    };
}