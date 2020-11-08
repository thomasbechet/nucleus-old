#include "renderpass.hpp"

#include "../engine/engine.hpp"

using namespace nuvk;

namespace
{
    static VkRenderPass CreateRenderPass(
        VkDevice device,
        VkFormat swapChainFormat
    )
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format         = swapChainFormat;
        colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments    = &colorAttachment;
        renderPassInfo.subpassCount    = 1;
        renderPassInfo.pSubpasses      = &subpass;

        VkRenderPass renderPass;
        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
            Engine::Interrupt(RenderPass::Section, "Failed to create render pass.");
        }
        return renderPass;
    }
}

struct RenderPass::Internal
{
    VkDevice device;
    VkRenderPass renderPass;

    Internal(
        const Device &device,
        const Swapchain &swapchain
    ) : device(device.getDevice())
    {
        renderPass = ::CreateRenderPass(device.getDevice(), swapchain.getFormat());    
    }
    ~Internal()
    {
        vkDestroyRenderPass(device, renderPass, nullptr);
    }
};

RenderPass::RenderPass(
    const Device &device,
    const Swapchain &swapchain
) : internal(MakeInternalPtr<Internal>(device, swapchain)) {}

VkRenderPass RenderPass::getRenderPass() const
{
    return internal->renderPass;
}