#include "commandpool.hpp"

#include "../engine/engine.hpp"

using namespace nuvk;

namespace
{
    static VkCommandPool CreateCommandPool(
        VkDevice device, 
        uint32_t queueFamilyIndex
    )
    {
        VkCommandPoolCreateInfo info{};
        info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        info.queueFamilyIndex = queueFamilyIndex;

        VkCommandPool commandPool;
        if (vkCreateCommandPool(device, &info, nullptr, &commandPool) != VK_SUCCESS) {
            Engine::Interrupt(CommandPool::Section, "Failed to create command pool.");
        }
        return commandPool;
    }

    static VkCommandBuffer BeginSingleCommandBuffer(
        VkCommandPool commandPool,
        VkDevice device
    )
    {
        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool        = commandPool;
        allocateInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer) != VK_SUCCESS) {
            Engine::Interrupt(CommandPool::Section, "Failed to allocate command buffer.");
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            Engine::Interrupt(CommandPool::Section, "Failed to begin command buffer.");
        }

        return commandBuffer;
    }

    static void EndSingleCommandBuffer(
        VkCommandBuffer commandBuffer, 
        VkQueue queue
    )
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount   = 0;
        submitInfo.pWaitSemaphores      = nullptr;
        submitInfo.pWaitDstStageMask    = nullptr;
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &commandBuffer;
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores    = nullptr;

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);
    }

    static std::vector<VkCommandBuffer> CreateCommandBuffers(
        VkDevice device,
        VkCommandPool commandPool,
        uint32_t count
    )
    {
        VkCommandBufferAllocateInfo info{};
        info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandPool        = commandPool;
        info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        info.commandBufferCount = count;

        std::vector<VkCommandBuffer> commandBuffers(count, VK_NULL_HANDLE);
        if (vkAllocateCommandBuffers(device, &info, commandBuffers.data()) != VK_SUCCESS) {
            Engine::Interrupt(CommandPool::Section, "Failed to allocate command buffers.");
        }
        return commandBuffers;
    }
}

struct CommandPool::Internal
{
    VkCommandPool commandPool;
    uint32_t queueFamilyIndex;
    const Device &device;

    Internal(
        const Device &device,
        uint32_t queueFamilyIndex
    ) : queueFamilyIndex(queueFamilyIndex), device(device)
    {
        commandPool = ::CreateCommandPool(device.getDevice(), queueFamilyIndex);
    }
    ~Internal()
    {
        vkDestroyCommandPool(device.getDevice(), commandPool, nullptr);   
    }
};

CommandPool::CommandPool(
    const Device &device,
    uint32_t queueFamilyIndex
) : internal(MakeInternalPtr<Internal>(device, queueFamilyIndex)) {}

VkCommandBuffer CommandPool::beginSingleCommandBuffer() const
{
    return ::BeginSingleCommandBuffer(internal->commandPool, internal->device.getDevice());
}
void CommandPool::endSingleCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue) const
{
    ::EndSingleCommandBuffer(commandBuffer, queue);
}

std::vector<VkCommandBuffer> CommandPool::createCommandBuffers(uint32_t count) const
{
    return ::CreateCommandBuffers(internal->device.getDevice(), internal->commandPool, count);
}
void CommandPool::destroyCommandBuffer(VkCommandBuffer commandBuffer) const
{
    vkFreeCommandBuffers(internal->device.getDevice(), internal->commandPool, 1, &commandBuffer);
}