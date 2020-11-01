#include "rendercontext.hpp"

#include "../utility/logger.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"

#include "../engine/engine.hpp"

using namespace nuvk;

namespace
{
    static std::vector<vk::UniqueFramebuffer> CreateFramebuffers(
        const vk::Device &device,
        const std::vector<vk::ImageView> imageViews,
        const vk::Extent2D extent,
        const vk::RenderPass &renderpass
    )
    {
        std::vector<vk::UniqueFramebuffer> framebuffers;

        for (const auto &imageView : imageViews) {
            std::array<vk::ImageView, 1> attachments{
                imageView
            };

            vk::FramebufferCreateInfo info(
                vk::FramebufferCreateFlags(),
                renderpass,
                static_cast<uint32_t>(attachments.size()),
                attachments.data(),
                extent.width,
                extent.height,
                1
            );

            framebuffers.push_back(device.createFramebufferUnique(info));
        }

        return framebuffers;
    }

    static vk::Rect2D CreateScissor(const Swapchain &swapchain)
    {
        vk::Offset2D offset{0, 0};
        return vk::Rect2D {
            offset,
            swapchain.getExtent()
        };
    }

    static vk::Viewport CreateViewport(const Swapchain &swapchain)
    {    
        return vk::Viewport {
            0.0f, 0.0f,
            static_cast<float>(swapchain.getExtent().width),
            static_cast<float>(swapchain.getExtent().height),
            0.0, 1.0f
        };
    }

    static std::array<vk::ClearValue, 1> CreateClearValues()
    {
        vk::ClearValue color;
        color.color = vk::ClearColorValue(std::array<float, 4>{
            0.0f, 0.0f, 0.0f, 1.0f
        });
        return std::array<vk::ClearValue, 1>{color};
    }
}

struct RenderContext::Internal
{
    std::unique_ptr<Swapchain> swapchain;
    std::unique_ptr<RenderPass> renderPass;

    std::vector<vk::UniqueFramebuffer> framebuffers;
    std::vector<vk::UniqueCommandBuffer> commandBuffers;

    const uint32_t maxInFlightFrames = 2;
    std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
    std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
    std::vector<vk::UniqueFence> inFlightFences;

    vk::Rect2D scissor;
    vk::Viewport viewport;
    std::array<vk::ClearValue, 1> clearValues;

    uint32_t currentFrameIndex = 0;
    uint32_t currentSwapchainImageIndex = 0;

    const Device &device;
    
    Internal(
        const Device &device,
        const PhysicalDevice &physicalDevice,
        const Surface &surface,
        const CommandPool &commandPool,
        uint32_t width, uint32_t height
    ) : device(device)
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

        commandBuffers = commandPool.createCommandBuffers(device.getDevice(), framebuffers.size());
        for (uint32_t i = 0; i < maxInFlightFrames; i++) {
            imageAvailableSemaphores.emplace_back(device.getDevice().createSemaphoreUnique({}));
            renderFinishedSemaphores.emplace_back(device.getDevice().createSemaphoreUnique({}));
            inFlightFences.emplace_back(device.getDevice().createFenceUnique({vk::FenceCreateFlagBits::eSignaled}));
        }
        
        scissor = ::CreateScissor(*swapchain);
        viewport = ::CreateViewport(*swapchain);
        clearValues = ::CreateClearValues();
    }
    ~Internal()
    {

    }

    const vk::CommandBuffer &getActiveCommandBuffer() const
    {
        return *commandBuffers[currentSwapchainImageIndex];
    }

    bool beginRender()
    {
        // acquire next frame resource
        const vk::Fence &inFlightFence{*inFlightFences[currentFrameIndex]};
        const vk::Semaphore &imageAvailableSemaphore{*imageAvailableSemaphores[currentFrameIndex]};

        if (device.getDevice().waitForFences(1, &inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max()) != vk::Result::eSuccess) {
            Engine::Interrupt("Failed to wait fence.");
        }
        device.getDevice().resetFences(1, &inFlightFence);

        try {
            currentSwapchainImageIndex = device.getDevice().acquireNextImageKHR(
                swapchain->getSwapchain(),
                std::numeric_limits<uint64_t>::max(),
                imageAvailableSemaphore,
                nullptr
            ).value;
        } catch (vk::OutOfDateKHRError &outOfDateError) {
            return false;
        }

        // record beginning of command buffer
        const vk::CommandBuffer &commandBuffer{getActiveCommandBuffer()};
        commandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

        vk::CommandBufferBeginInfo commandBufferBeginInfo(
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            nullptr
        );
        if (commandBuffer.begin(&commandBufferBeginInfo) != vk::Result::eSuccess) {
            Engine::Interrupt("Failed to begin command buffer.");
        }

        commandBuffer.setScissor(0, 1, &scissor);
        commandBuffer.setViewport(0, 1, &viewport);

        vk::RenderPassBeginInfo renderPassBeginInfo(
            renderPass->getRenderPass(),
            framebuffers[currentSwapchainImageIndex].get(),
            scissor,
            1,
            clearValues.data()
        );
        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        return true;
    }
    bool endRender()
    {
        // record end command buffer
        const vk::CommandBuffer &commandBuffer{getActiveCommandBuffer()};
        commandBuffer.endRenderPass();
        commandBuffer.end();

        // submit command buffer
        const vk::Fence &inFlightFence{*inFlightFences[currentFrameIndex]};
        const vk::Semaphore &imageAvailableSemaphore{*imageAvailableSemaphores[currentFrameIndex]};
        const vk::Semaphore &renderFinishedSemaphore{*renderFinishedSemaphores[currentFrameIndex]};

        vk::SubmitInfo submitInfo = {};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

        try {
            device.getGraphicsQueue().submit(submitInfo, inFlightFence);
        } catch (vk::SystemError &error) {
            Engine::Interrupt("Failed to submit draw command buffer.");
        }

        vk::PresentInfoKHR presentInfo = {};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain->getSwapchain();
        presentInfo.pImageIndices = &currentSwapchainImageIndex;
        presentInfo.pResults = nullptr;

        try {
            if (device.getPresentQueue().presentKHR(presentInfo) == vk::Result::eSuboptimalKHR) {
                return false;
            }
        } catch (vk::OutOfDateKHRError &outOfDateError) {
            return false;
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

const vk::Viewport &RenderContext::getViewport() const
{
    return internal->viewport;
}
const vk::Rect2D &RenderContext::getScissor() const
{
    return internal->scissor;
}
const vk::RenderPass &RenderContext::getRenderPass() const
{
    return internal->renderPass->getRenderPass();
}
const vk::CommandBuffer &RenderContext::getActiveCommandBuffer() const
{
    return internal->getActiveCommandBuffer();
}
vk::Extent2D RenderContext::getSwapchainExtent() const
{
    return internal->swapchain->getExtent();
}