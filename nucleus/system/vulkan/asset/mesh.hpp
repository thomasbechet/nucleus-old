#pragma once

#include "../utility/internalptr.hpp"
#include "../vulkan/buffer.hpp"
#include "../vulkan/memoryallocator.hpp"

#include "../../../core/nucleus.h"

namespace nuvk
{
    class Mesh
    {
    public:
        Mesh(
            const nu_renderer_mesh_create_info_t &info,
            const Device &device,
            const MemoryAllocator &memoryAllocator,
            const CommandPool &graphicsCommandPool
        );

        const Buffer &getVertexBuffer() const;
        uint32_t getVertexCount() const;

        static VkVertexInputBindingDescription GetVertexInputBindingDescription();
        static std::vector<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions();

    private:
        struct Internal;
        InternalPtr<Internal> internal;
    };
};