#include "mesh.hpp"

#include "../utility/logger.hpp"

#include <memory>

using namespace nuvk;

namespace
{
    struct Vertex
    {
        nu_vec3_t position;
        nu_vec2_t uv;
        nu_vec3_t color;
    };
}

struct Mesh::Internal
{
    std::unique_ptr<Buffer> vertexBuffer;
    uint32_t vertexCount;

    Internal(
        const nu_renderer_mesh_create_info_t &info,
        const Device &device,
        const MemoryAllocator &memoryAllocator,
        const CommandPool &graphicsCommandPool
    )
    {
        std::vector<Vertex> vertices;

        if (info.indice_count) {
            vertices.resize(info.indice_count);
            if (info.position_indices) {
                for (uint32_t i = 0; i < info.indice_count; i++) {
                    nu_vec3_copy(info.positions[info.position_indices[i]], vertices[i].position);
                }
            }
            if (info.uv_indices) {
                for (uint32_t i = 0; i < info.indice_count; i++) {
                    nu_vec2_copy(info.uvs[info.uv_indices[i]], vertices[i].uv);
                }
            }
            
        } else {
            vertices.resize(info.vertex_count);
            if (info.positions) {
                for (uint32_t i = 0; i < info.vertex_count; i++) {
                    nu_vec3_copy(info.positions[i], vertices[i].position);
                }
            }
            if (info.uvs) {
                for (uint32_t i = 0; i < info.vertex_count; i++) {
                    nu_vec2_copy(info.uvs[i], vertices[i].uv);
                }
            }
        }
        vertexCount = vertices.size();

        vertexBuffer = std::make_unique<Buffer>(
            Buffer::CreateDeviceLocalBuffer(
                memoryAllocator,
                graphicsCommandPool,
                device.getGraphicsQueue(),
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                sizeof(Vertex) * vertices.size(),
                vertices.data()
            )
        );
    }
    ~Internal()
    {

    }
};

Mesh::Mesh(
    const nu_renderer_mesh_create_info_t &info,
    const Device &device,
    const MemoryAllocator &memoryAllocator,
    const CommandPool &graphicsCommandPool
) : internal(MakeInternalPtr<Internal>(info, device, memoryAllocator, graphicsCommandPool)) {}

const Buffer &Mesh::getVertexBuffer() const
{
    return *internal->vertexBuffer;
}
uint32_t Mesh::getVertexCount() const
{
    return internal->vertexCount;
}

VkVertexInputBindingDescription Mesh::GetVertexInputBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription;
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}
std::vector<VkVertexInputAttributeDescription> Mesh::GetVertexAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    attributeDescriptions.resize(3);

    attributeDescriptions[0].binding  = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset   = offsetof(Vertex, position);

    attributeDescriptions[1].binding  = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset   = offsetof(Vertex, uv);

    attributeDescriptions[2].binding  = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset   = offsetof(Vertex, color);

    return attributeDescriptions;
}