#include "commandpool.hpp"

using namespace nuvk;

namespace
{
    vk::UniqueCommandPool CreateCommandPool(const vk::Device &device, uint32_t queueIndex)
    {
        vk::CommandPoolCreateInfo info(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueIndex
        );

        return device.createCommandPoolUnique(info);
    }

    vk::UniqueCommandBuffer BeginCommandBuffer(
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

    void EndCommandBuffer(
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
}

struct CommandPool::Internal
{
    vk::UniqueCommandPool commandPool;
    uint32_t queueIndex;

    Internal(
        const vk::Device &device,
        uint32_t queueIndex
    ) : queueIndex(queueIndex)
    {
        commandPool = ::CreateCommandPool(device, queueIndex);
    }
    ~Internal()
    {
        
    }
};

CommandPool::CommandPool(
    const vk::Device &device,
    uint32_t queueIndex
) : internal(MakeInternalPtr<Internal>(device, queueIndex)) {}

vk::UniqueCommandBuffer CommandPool::beginCommandBuffer(const vk::Device &device) const
{
    return ::BeginCommandBuffer(*internal->commandPool, device);
}
void CommandPool::endCommandBuffer(const vk::CommandBuffer &commandBuffer, const vk::Queue &queue) const
{
    ::EndCommandBuffer(commandBuffer, queue);
}