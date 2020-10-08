#include "engine.hpp"

#include <nucleus/nucleus.h>
#include "../utility/logger.hpp"

#include <set>
#include <fstream>

using namespace nuvk;

Engine::Engine()
{
    initialize();
}
Engine::~Engine()
{
    terminate();
}

void Engine::render()
{
    drawFrame();
}

void Engine::Interrupt(std::string msg)
{
    nu_interrupt(("[NUVK] " + msg + "\n").c_str());
}
std::vector<const char*> Engine::GetRequiredExtensions(GLFWInterface &glfwInterface)
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwInterface->get_required_instance_extensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (Engine::USE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
std::vector<const char*> Engine::GetRequiredValidationLayers()
{
    return {
        "VK_LAYER_KHRONOS_validation"
    };
}
std::vector<const char*> Engine::GetRequiredDeviceExtensions()
{
    return {
        "VK_KHR_swapchain"
    };
}
bool Engine::CheckValidationLayerSupport()
{
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : Engine::GetRequiredValidationLayers()) {
        bool layerFound = false;
        for (const auto &layerProperties : availableLayers) {
            if (std::string(layerName) == std::string((const char*)layerProperties.layerName)) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}
bool Engine::CheckDeviceExtensionSupport(vk::PhysicalDevice device)
{
    std::vector<const char*> deviceExtensions = Engine::GetRequiredDeviceExtensions();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : device.enumerateDeviceExtensionProperties()) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
bool Engine::IsDeviceSuitable(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = Engine::FindQueueFamilies(device, surface);

    bool extensionsSupported = Engine::CheckDeviceExtensionSupport(device);
    
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = Engine::QuerySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    
    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}
QueueFamilyIndices Engine::FindQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    auto queueFamilies = device.getQueueFamilyProperties();
    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        if (queueFamily.queueCount > 0 && device.getSurfaceSupportKHR(i, surface)) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) break;
    
        i++;
    }
    return indices;
}
SwapChainSupportDetails Engine::QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}
vk::SurfaceFormatKHR Engine::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}
vk::PresentModeKHR Engine::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes)
{
    vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        } else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}
vk::Extent2D Engine::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        vk::Extent2D actualExtent = {static_cast<uint32_t>(Engine::WIDTH), static_cast<uint32_t>(Engine::HEIGHT)};
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}
vk::UniqueShaderModule Engine::CreateShaderModule(const vk::UniqueDevice &device, const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        Engine::Interrupt("Failed to open file '" + filename + "'");
    }

    uint32_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return device->createShaderModuleUnique({
        vk::ShaderModuleCreateFlagBits(),
        buffer.size(),
        reinterpret_cast<const uint32_t*>(buffer.data())
    });
}

void Engine::initialize()
{
    Logger::Info("Create instance..."); createInstance();
    Logger::Info("Setup debug callback..."); setupDebugCallback();
    Logger::Info("Create surface..."); createSurface();
    Logger::Info("Pick physical device..."); pickPhysicalDevice();
    Logger::Info("Create logical device..."); createLogicalDevice();
    Logger::Info("Create swapchain..."); createSwapChain();
    Logger::Info("Create image views..."); createImageViews();
    Logger::Info("Create render pass..."); createRenderPass();
    Logger::Info("Create graphics pipeline..."); createGraphicsPipeline();
    Logger::Info("Create framebuffers..."); createFramebuffers();
    Logger::Info("Create command pool..."); createCommandPool();
    Logger::Info("Create command buffers..."); createCommandBuffers();
    Logger::Info("Create synchronization objects..."); createSyncObjects();
}
void Engine::terminate()
{
    m_device->waitIdle();

    m_swapChain.reset(); /* destruction order */
    m_instance->destroySurfaceKHR(m_surface); /* not using vk::UniqueSurfaceKHR */
}
void Engine::drawFrame()
{
    if (m_device->waitForFences(1, &(*m_inFlightFences[m_currentFrame]), VK_TRUE, std::numeric_limits<uint64_t>::max()) != vk::Result::eSuccess) {
        Engine::Interrupt("Failed to wait fences.");
    }
    m_device->resetFences(1, &(*m_inFlightFences[m_currentFrame]));

    uint32_t imageIndex = m_device->acquireNextImageKHR(*m_swapChain, std::numeric_limits<uint64_t>::max(), *m_imageAvailableSemaphores[m_currentFrame], nullptr).value;

    vk::SubmitInfo submitInfo = {};

    vk::Semaphore waitSemaphores[] = {*m_imageAvailableSemaphores[m_currentFrame]};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(*m_commandBuffers[imageIndex]);

    vk::Semaphore signalSemaphores[] = {*m_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    try {
        m_graphicsQueue.submit(submitInfo, *m_inFlightFences[m_currentFrame]);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to submit draw command buffer.");
    }

    vk::PresentInfoKHR presentInfo = {};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = {*m_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    if (m_presentQueue.presentKHR(presentInfo) != vk::Result::eSuccess) {
        Engine::Interrupt("Failed to presentKHR.");
    }

    m_currentFrame = (m_currentFrame + 1) % Engine::MAX_FRAMES_IN_FLIGHT;
}

void Engine::createInstance()
{
    if (Engine::USE_VALIDATION_LAYERS && !Engine::CheckValidationLayerSupport()) {
        Engine::Interrupt("Validation layers requested, but not available.");
    }

    auto appInfo = vk::ApplicationInfo(
        "Vulkan Renderer",
        VK_MAKE_VERSION(1, 0, 0),
        "Nucleus Engine",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_0
    );

    auto extensions = Engine::GetRequiredExtensions(m_glfwInterface);

    auto createInfo = vk::InstanceCreateInfo(
        vk::InstanceCreateFlags(),
        &appInfo,
        0, nullptr, // enabled layers
        static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
    );

    auto validationLayers = Engine::GetRequiredValidationLayers();
    if (Engine::USE_VALIDATION_LAYERS) { // adding enabled layers
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    try {
        m_instance = vk::createInstanceUnique(createInfo, nullptr);
    } catch(vk::SystemError &err) {
        Logger::Fatal(err.what());
        Engine::Interrupt("Failed to create instance.");
    }
}
void Engine::setupDebugCallback()
{
    if (!Engine::USE_VALIDATION_LAYERS) return;
    m_debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(m_instance);
}
void Engine::createSurface()
{
    VkSurfaceKHR rawSurface;
    VkInstance instance = *m_instance;
    if (m_glfwInterface->create_window_surface(&instance, &rawSurface) != NU_SUCCESS) {
        Engine::Interrupt("Failed to create surface from GLFW.");
    }
    m_surface = rawSurface;
}
void Engine::pickPhysicalDevice()
{
    auto devices = m_instance->enumeratePhysicalDevices();
    if (devices.size() == 0) {
        Engine::Interrupt("Failed to find GPUs with Vulkan support.");
    }

    for (const auto &device : devices) {
        if (Engine::IsDeviceSuitable(device, m_surface)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (!m_physicalDevice) {
        Engine::Interrupt("Failed to find suitable GPU.");
    }
}
void Engine::createLogicalDevice()
{
    QueueFamilyIndices indices = Engine::FindQueueFamilies(m_physicalDevice, m_surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        queueCreateInfos.push_back({
            vk::DeviceQueueCreateFlags(),
            queueFamily,
            1, // queue count
            &queuePriority
        });
    }

    auto createInfo = vk::DeviceCreateInfo(
        vk::DeviceCreateFlags(),
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data()
    );

    auto deviceFeatures = vk::PhysicalDeviceFeatures();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    auto deviceExtensions = Engine::GetRequiredDeviceExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames =  deviceExtensions.data();

    auto layers = Engine::GetRequiredValidationLayers();
    if (Engine::USE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
    }

    try {
        m_device = m_physicalDevice.createDeviceUnique(createInfo);
    } catch(vk::SystemError &err) {
        Engine::Interrupt("Failed to create logical device.");
    }

    m_graphicsQueue = m_device->getQueue(indices.graphicsFamily.value(), 0);
    m_presentQueue = m_device->getQueue(indices.presentFamily.value(), 0);
}
void Engine::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = Engine::QuerySwapChainSupport(m_physicalDevice, m_surface);

    vk::SurfaceFormatKHR surfaceFormat = Engine::ChooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = Engine::ChooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = Engine::ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo(
        vk::SwapchainCreateFlagsKHR(),
        m_surface,
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1, // imageArrayLayers
        vk::ImageUsageFlagBits::eColorAttachment
    );

    QueueFamilyIndices indices = Engine::FindQueueFamilies(m_physicalDevice, m_surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

    try {
        m_swapChain = m_device->createSwapchainKHRUnique(createInfo);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create swapchain.");
    }

    m_swapChainImages = m_device->getSwapchainImagesKHR(*m_swapChain);
    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}
void Engine::createImageViews()
{
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo = {};
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = m_swapChainImageFormat;
        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;
        createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        try {
            m_swapChainImageViews[i] = m_device->createImageViewUnique(createInfo);
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create image views.");
        }
    }
}
void Engine::createRenderPass()
{
    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.format = m_swapChainImageFormat;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass = {};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    vk::RenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    try {
        m_renderPass = m_device->createRenderPassUnique(renderPassInfo);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create render pass.");
    }
}
void Engine::createGraphicsPipeline()
{
    auto vertShaderModule = Engine::CreateShaderModule(m_device, "engine/shader/spirv/shader.vert.spv");
    auto fragShaderModule = Engine::CreateShaderModule(m_device, "engine/shader/spirv/shader.frag.spv");

    vk::PipelineShaderStageCreateInfo shaderStages[] = {
        {
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eVertex,
            *vertShaderModule,
            "main"
        },
        {
            vk::PipelineShaderStageCreateFlagBits(),
            vk::ShaderStageFlagBits::eFragment,
            *fragShaderModule,
            "main"
        }
    };

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    vk::Viewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_swapChainExtent.width;
    viewport.height = (float)m_swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vk::Rect2D scissor = {};
    scissor.offset.x = 0.0f;
    scissor.offset.y = 0.0f;
    scissor.extent = m_swapChainExtent;

    vk::PipelineViewportStateCreateInfo viewportState = {};
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    vk::PipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    try {
        m_pipelineLayout = m_device->createPipelineLayoutUnique(pipelineLayoutInfo);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create pipeline layout.");
    }

    vk::GraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = *m_pipelineLayout;
    pipelineInfo.renderPass = *m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;

    try {
        m_graphicsPipeline = m_device->createGraphicsPipelineUnique(nullptr, pipelineInfo).value;
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create graphics pipeline.");
    }
}
void Engine::createFramebuffers()
{
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (uint32_t i = 0; i < m_swapChainImageViews.size(); i++) {
        vk::ImageView attachments[] = {
            *m_swapChainImageViews[i]
        };

        vk::FramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.renderPass = *m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swapChainExtent.width;
        framebufferInfo.height = m_swapChainExtent.height;
        framebufferInfo.layers = 1;

        try {
            m_swapChainFramebuffers[i] = m_device->createFramebufferUnique(framebufferInfo);
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to create framebuffer.");
        }
    }
}
void Engine::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = Engine::FindQueueFamilies(m_physicalDevice, m_surface);

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    try {
        m_commandPool = m_device->createCommandPoolUnique(poolInfo);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create command pool.");
    }
}
void Engine::createCommandBuffers()
{
    m_commandBuffers.resize(m_swapChainFramebuffers.size());

    vk::CommandBufferAllocateInfo allocInfo = {};
    allocInfo.commandPool = *m_commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

    try {
        m_commandBuffers = m_device->allocateCommandBuffersUnique(allocInfo);
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to allocate command buffers.");
    }

    for (uint32_t i = 0; i < m_commandBuffers.size(); i++) {
        vk::CommandBufferBeginInfo beginInfo = {};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

        try {
            m_commandBuffers[i]->begin(beginInfo);
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to begin recording command buffer.");
        }

        vk::RenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.renderPass = *m_renderPass;
        renderPassInfo.framebuffer = *m_swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset.x = 0;
        renderPassInfo.renderArea.offset.y = 0;
        renderPassInfo.renderArea.extent = m_swapChainExtent;

        vk::ClearValue clearColor = {std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        m_commandBuffers[i]->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
        m_commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, *m_graphicsPipeline);
        m_commandBuffers[i]->draw(3, 1, 0, 0);
        m_commandBuffers[i]->endRenderPass();

        try {
            m_commandBuffers[i]->end();
        } catch (vk::SystemError &err) {
            Engine::Interrupt("Failed to record command buffer.");
        }
    }
}
void Engine::createSyncObjects()
{
    m_imageAvailableSemaphores.resize(Engine::MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(Engine::MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(Engine::MAX_FRAMES_IN_FLIGHT);

    try {
        for (uint32_t i = 0; i < Engine::MAX_FRAMES_IN_FLIGHT; i++) {
            m_imageAvailableSemaphores[i] = m_device->createSemaphoreUnique({});
            m_renderFinishedSemaphores[i] = m_device->createSemaphoreUnique({});
            m_inFlightFences[i] = m_device->createFenceUnique({vk::FenceCreateFlagBits::eSignaled});
        }
    } catch (vk::SystemError &err) {
        Engine::Interrupt("Failed to create synchronization objects.");
    }
}