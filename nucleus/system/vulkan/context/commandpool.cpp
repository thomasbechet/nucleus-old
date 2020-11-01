#include "commandpool.hpp"

using namespace nuvk;

namespace
{
    static vk::UniqueCommandPool CreateCommandPool(const vk::Device &device, uint32_t queueIndex)
    {
        vk::CommandPoolCreateInfo info(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueIndex
        );

        return device.createCommandPoolUnique(info);
    }

    static vk::UniqueCommandBuffer BeginCommandBuffer(
        const vk::CommandPool &commandPool,
        const vk::Device &device
    )
    {
        vk::CommandBufferAllocateInfo allocateInfo(
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        vk::UniqueCommandBuffer commandBuffer(
            std::move(device.allocateCommandBuffersUnique(allocateInfo)[0])
        );

        vk::CommandBufferBeginInfo beginInfo(
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            nullptr
        );

        commandBuffer->begin(beginInfo);

        return commandBuffer;
    }

    static void EndCommandBuffer(
        const vk::CommandBuffer &commandBuffer, 
        const vk::Queue &queue
    )
    {
        commandBuffer.end();

        vk::SubmitInfo submitInfo(
            0,
            nullptr,
            nullptr,
            1,
            &commandBuffer,
            0, 
            nullptr
        );

        queue.submit(1, &submitInfo, vk::Fence());
        queue.waitIdle();
    }

    static std::vector<vk::UniqueCommandBuffer> CreateCommandBuffer(
        const vk::Device &device,
        const vk::CommandPool &commandPool,
        uint32_t count
    )
    {
        vk::CommandBufferAllocateInfo info(
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            count
        );

        return device.allocateCommandBuffersUnique(info);
    }
}

struct CommandPool::Internal
{
    vk::UniqueCommandPool commandPool;
    uint32_t queueIndex;
    const Device &device;

    Internal(
        const Device &device,
        uint32_t queueIndex
    ) : queueIndex(queueIndex), device(device)
    {
        commandPool = ::CreateCommandPool(device.getDevice(), queueIndex);
    }
    ~Internal()
    {
        
    }
};

CommandPool::CommandPool(
    const Device &device,
    uint32_t queueIndex
) : internal(MakeInternalPtr<Internal>(device, queueIndex)) {}

vk::UniqueCommandBuffer CommandPool::beginCommandBuffer() const
{
    return ::BeginCommandBuffer(*internal->commandPool, internal->device.getDevice());
}
void CommandPool::endCommandBuffer(const vk::CommandBuffer &commandBuffer, const vk::Queue &queue) const
{
    ::EndCommandBuffer(commandBuffer, queue);
}

std::vector<vk::UniqueCommandBuffer> CommandPool::createCommandBuffers(const vk::Device &device, uint32_t count) const
{
    return ::CreateCommandBuffer(device, *internal->commandPool, count);
}