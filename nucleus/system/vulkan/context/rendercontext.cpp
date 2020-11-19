#include "rendercontext.hpp"

#include "../utility/logger.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"

#include "../engine/engine.hpp"

using namespace nuvk;

namespace
{
    static std::vector<VkFramebuffer> CreateFramebuffers(
        VkDevice device,
        const std::vector<VkImageView> &imageViews,
        VkExtent2D extent,
        VkRenderPass renderpass
    )
    {
        std::vector<VkFramebuffer> framebuffers;

        for (const auto &imageView : imageViews) {
            std::array<VkImageView, 1> attachments{
                imageView
            };

            VkFramebufferCreateInfo info{};
            info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            info.renderPass      = renderpass;
            info.attachmentCount = static_cast<uint32_t>(attachments.size());
            info.pAttachments    = attachments.data();
            info.width           = extent.width;
            info.height          = extent.height;
            info.layers          = 1;

            VkFramebuffer framebuffer;
            if (vkCreateFramebuffer(device, &info, nullptr, &framebuffer) != VK_SUCCESS) {
                Engine::Interrupt(RenderContext::Section, "Failed to create framebuffer.");
            }
            framebuffers.push_back(framebuffer);
        }

        return framebuffers;
    }

    static VkRect2D CreateScissor(const Swapchain &swapchain)
    {
        VkRect2D rect;
        rect.offset.x = 0;
        rect.offset.y = 0;
        rect.extent   = swapchain.getExtent();
        return rect;
    }

    static VkViewport CreateViewport(const Swapchain &swapchain)
    {    
        VkViewport viewport;
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = static_cast<float>(swapchain.getExtent().width);
        viewport.height   = static_cast<float>(swapchain.getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        return viewport;
    }

    static std::array<VkClearValue, 1> CreateClearValues()
    {
        VkClearValue color = {0.0f, 0.0f, 0.0f, 1.0f};
        return std::array<VkClearValue, 1>{color};
    }
}

struct RenderContext::Internal
{
    std::unique_ptr<Swapchain> swapchain;
    std::unique_ptr<RenderPass> renderPass;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkCommandBuffer> commandBuffers;

    const uint32_t maxInFlightFrames = 2;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    VkRect2D scissor;
    VkViewport viewport;
    std::array<VkClearValue, 1> clearValues;

    uint32_t currentFrameIndex = 0;
    uint32_t currentSwapchainImageIndex = 0;

    const Device &device;
    const CommandPool &commandPool;
    
    Internal(
        const Device &device,
        const PhysicalDevice &physicalDevice,
        const Surface &surface,
        const CommandPool &commandPool,
        uint32_t width, uint32_t height
    ) : device(device), commandPool(commandPool)
    {
        // create swapchain
        Logger::Info(RenderPass::Section, "Creating swapchain...");
        swapchain = std::make_unique<Swapchain>(
            device,
            physicalDevice,
            surface,
            width, height
        );

        // create renderpass
        Logger::Info(RenderPass::Section, "Creating renderpass...");
        renderPass = std::make_unique<RenderPass>(
            device,
            *swapchain
        );

        // create framebuffers
        Logger::Info(RenderPass::Section, "Creating swapchain framebuffers...");
        framebuffers = ::CreateFramebuffers(
            device.getDevice(),
            swapchain->getImageViews(),
            swapchain->getExtent(),
            renderPass->getRenderPass()
        );

        commandBuffers = commandPool.createCommandBuffers(framebuffers.size());
        for (uint32_t i = 0; i < maxInFlightFrames; i++) {
            VkSemaphore semaphore;
            VkFence fence;

            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr, &semaphore);
            imageAvailableSemaphores.emplace_back(semaphore);
            vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr, &semaphore);
            renderFinishedSemaphores.emplace_back(semaphore);

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            vkCreateFence(device.getDevice(), &fenceInfo, nullptr, &fence);
            inFlightFences.emplace_back(fence);
        }
        
        scissor = ::CreateScissor(*swapchain);
        viewport = ::CreateViewport(*swapchain);
        clearValues = ::CreateClearValues();
    }
    ~Internal()
    {
        for (auto fence : inFlightFences) {
            vkDestroyFence(device.getDevice(), fence, nullptr);
        }
        for (auto semaphore : renderFinishedSemaphores) {
            vkDestroySemaphore(device.getDevice(), semaphore, nullptr);
        }
        for (auto semaphore : imageAvailableSemaphores) {
            vkDestroySemaphore(device.getDevice(), semaphore, nullptr);
        }

        for (auto commandBuffer : commandBuffers) {
            commandPool.destroyCommandBuffer(commandBuffer);
        }
        for (auto framebuffer : framebuffers) {
            vkDestroyFramebuffer(device.getDevice(), framebuffer, nullptr);
        }

        renderPass.reset();
        swapchain.reset();
    }

    VkCommandBuffer getActiveCommandBuffer() const
    {
        return commandBuffers[currentSwapchainImageIndex];
    }

    bool beginRender()
    {
        // acquire next frame resource
        VkFence inFlightFence = inFlightFences[currentFrameIndex];
        VkSemaphore imageAvailableSemaphore = imageAvailableSemaphores[currentFrameIndex];

        if (vkWaitForFences(device.getDevice(), 1, &inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max()) != VK_SUCCESS) {
            Engine::Interrupt(RenderContext::Section, "Failed to wait fence.");
        }
        vkResetFences(device.getDevice(), 1, &inFlightFence);

        VkResult result = vkAcquireNextImageKHR(
            device.getDevice(),
            swapchain->getSwapchain(),
            std::numeric_limits<uint64_t>::max(),
            imageAvailableSemaphore,
            nullptr,
            &currentSwapchainImageIndex
        );
        if (result != VK_SUCCESS) {
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                return false;
            } else {
                Engine::Interrupt(RenderContext::Section, "Failed to acquire next image KHR.");
            }
        }

        // record beginning of command buffer
        VkCommandBuffer commandBuffer = getActiveCommandBuffer();
        vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

        VkCommandBufferBeginInfo commandBufferBeginInfo{};
        commandBufferBeginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;
        if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS) {
            Engine::Interrupt(RenderContext::Section, "Failed to begin command buffer.");
        }

        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass      = renderPass->getRenderPass();
        renderPassBeginInfo.framebuffer     = framebuffers[currentSwapchainImageIndex];
        renderPassBeginInfo.renderArea      = scissor;
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues    = clearValues.data();
        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        return true;
    }
    bool endRender()
    {
        // record end command buffer
        VkCommandBuffer commandBuffer = getActiveCommandBuffer();
        vkCmdEndRenderPass(commandBuffer);
        vkEndCommandBuffer(commandBuffer);

        // submit command buffer
        VkFence inFlightFence = inFlightFences[currentFrameIndex];
        VkSemaphore imageAvailableSemaphore = imageAvailableSemaphores[currentFrameIndex];
        VkSemaphore renderFinishedSemaphore = renderFinishedSemaphores[currentFrameIndex];

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo{};
        submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount   = 1;
        submitInfo.pWaitSemaphores      = &imageAvailableSemaphore;
        submitInfo.pWaitDstStageMask    = waitStages;
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = &renderFinishedSemaphore;

        if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
            Engine::Interrupt(RenderContext::Section, "Failed to submit draw command buffer.");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = &renderFinishedSemaphore;
        presentInfo.swapchainCount     = 1;
        VkSwapchainKHR swapchains[] = {swapchain->getSwapchain()};
        presentInfo.pSwapchains        = swapchains;
        presentInfo.pImageIndices      = &currentSwapchainImageIndex;
        presentInfo.pResults           = nullptr;

        VkResult result = vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);
        if (result != VK_SUCCESS) {
            if (result == VK_SUBOPTIMAL_KHR) {
                return false;
            } else {
                return false;
            }
        }

        currentFrameIndex = (currentFrameIndex + 1) % maxInFlightFrames;

        return true;
    }
};

RenderContext::RenderContext(
    const Device &device,
    const PhysicalDevice &physicalDevice,
    const Surface &surface,
    const CommandPool &commandPool,
    uint32_t width, uint32_t height
) : internal(MakeInternalPtr<Internal>(device, physicalDevice, surface, commandPool, width, height)) {}

bool RenderContext::beginRender()
{
    return internal->beginRender();
}
bool RenderContext::endRender()
{
    return internal->endRender();
}

VkViewport RenderContext::getViewport() const
{
    return internal->viewport;
}
VkRect2D RenderContext::getScissor() const
{
    return internal->scissor;
}
VkExtent2D RenderContext::getExtent() const
{
    return internal->swapchain->getExtent();
}
VkRenderPass RenderContext::getRenderPass() const
{
    return internal->renderPass->getRenderPass();
}

VkCommandBuffer RenderContext::getActiveCommandBuffer() const
{
    return internal->getActiveCommandBuffer();
}
uint32_t RenderContext::getActiveFrameResourceIndex() const
{
    return internal->currentSwapchainImageIndex;
}
uint32_t RenderContext::getFrameResourceCount() const
{
    return internal->framebuffers.size();
}