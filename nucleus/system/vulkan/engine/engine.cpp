#include "engine.hpp"

#include <nucleus/nucleus.h>
#include "../utility/logger.hpp"

#include "../context/debugutilsmessenger.hpp"
#include "../context/instance.hpp"
#include "../context/physicaldevice.hpp"
#include "../context/device.hpp"
#include "../context/surface.hpp"
#include "../context/memoryallocator.hpp"
#include "../context/rendercontext.hpp"
#include "../context/pipeline.hpp"
#include "../context/windowinterface.hpp"
#include "../context/buffer.hpp"

#include <cstring>

using namespace nuvk;

namespace
{
    
}

struct Engine::Internal
{
    // Context
    const bool enableValidationLayers = true; 
    std::unique_ptr<WindowInterface> windowInterface;
    std::unique_ptr<Instance> instance;
    std::unique_ptr<DebugUtilsMessenger> debugUtilsMessenger;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physicalDevice;
    std::unique_ptr<Device> device;
    std::unique_ptr<CommandPool> graphicsCommandPool;
    std::unique_ptr<MemoryAllocator> memoryAllocator;

    // RenderContext
    bool renderContextOutOfDate = false;
    std::unique_ptr<RenderContext> renderContext;
    std::unique_ptr<Pipeline> pipeline;

    // Static Resources
    std::unique_ptr<Buffer> vertexBuffer;
    std::unique_ptr<Buffer> indiceBuffer;

    // Frame Resources
    uint32_t previousFrameCount = 0;
    struct UBO 
    {
        std::unique_ptr<Buffer> buffer;
        UniformBufferObject *ptr;
    };
    std::vector<UBO> uniformBuffers;

    Internal()
    {
        createContext();
        createStaticResources();
        createRenderContext();
        createFrameResources();
    }
    ~Internal()
    {
        vkDeviceWaitIdle(device->getDevice());
        
        destroyFrameResources();
        destroyRenderContext();
        destroyStaticResources();
        destroyContext();
    }

    void createContext()
    {
        // Get window interface
        Logger::Info(Engine::Section, "Getting window interface...");
        windowInterface = std::make_unique<WindowInterface>();
        
        // Create instance
        Logger::Info(Engine::Section, "Creating instance...");
        instance = std::make_unique<Instance>(*windowInterface, enableValidationLayers);
        
        if (enableValidationLayers) {
            // Create debug utils messenger
            Logger::Info(Engine::Section, "Creating debug utils messenger...");
            debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(*instance);
        }
        
        // Create surface
        Logger::Info(Engine::Section, "Creating surface...");
        surface = std::make_unique<Surface>(
            *instance,
            *windowInterface
        );

        // Create physical device
        Logger::Info(Engine::Section, "Picking physical device...");
        physicalDevice = std::make_unique<PhysicalDevice>(
            *instance,
            *surface
        );

        // Create device
        Logger::Info(Engine::Section, "Creating device...");
        device = std::make_unique<Device>(
            *physicalDevice,
            *surface,
            enableValidationLayers
        );

        // Create commandpool
        Logger::Info(Engine::Section, "Create command pool...");
        graphicsCommandPool = std::make_unique<CommandPool>(
            *device,
            device->getGraphicsQueueIndex()
        );

        // Create memory allocator
        Logger::Info(Engine::Section, "Create memory allocator...");
        memoryAllocator = std::make_unique<MemoryAllocator>(
            *instance,
            *physicalDevice,
            *device
        );
    }
    void createStaticResources()
    {
        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };

        vertexBuffer = std::make_unique<Buffer>(
            Buffer::CreateDeviceLocalBuffer(
                *memoryAllocator,
                *graphicsCommandPool,
                device->getGraphicsQueue(),
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                sizeof(Vertex) * vertices.size(),
                vertices.data()
            )
        );

        const std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        indiceBuffer = std::make_unique<Buffer>(
            Buffer::CreateDeviceLocalBuffer(
                *memoryAllocator,
                *graphicsCommandPool,
                device->getGraphicsQueue(),
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                sizeof(uint32_t) * indices.size(),
                indices.data()
            )
        );
    }
    void createRenderContext()
    {
        // Create rendercontext
        Logger::Info(Engine::Section, "Creating render context...");
        uint32_t width, height;
        nu_window_get_size(&width, &height);
        renderContext = std::make_unique<RenderContext>(
            *device,
            *physicalDevice,
            *surface,
            *graphicsCommandPool,
            width, height
        );

        // Create pipeline
        Logger::Info(Engine::Section, "Creating pipeline...");
        pipeline = std::make_unique<Pipeline>(
            *device,
            *renderContext
        );
    }
    void createFrameResources()
    {
        const uint32_t resourceCount = renderContext->getFrameResourceCount();
        if (resourceCount == previousFrameCount) {
            return;
        } else {
            previousFrameCount = resourceCount;
        }

        // Create resources
        uniformBuffers.resize(resourceCount);
        for (auto &ubo : uniformBuffers) {
            ubo.buffer = std::make_unique<Buffer>(
                *memoryAllocator,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VMA_MEMORY_USAGE_CPU_TO_GPU,
                sizeof(UniformBufferObject),
                nullptr
            );
            ubo.ptr = static_cast<UniformBufferObject*>(ubo.buffer->map());
        }
    }

    void destroyFrameResources()
    {
        uniformBuffers.clear();
    }
    void destroyRenderContext()
    {
        pipeline.reset();
        renderContext.reset();
    }
    void destroyStaticResources()
    {
        indiceBuffer.reset();
        vertexBuffer.reset();
    }
    void destroyContext()
    {
        memoryAllocator.reset();
        graphicsCommandPool.reset();
        device.reset();
        physicalDevice.reset();
        surface.reset();
        if (enableValidationLayers) {
            debugUtilsMessenger.reset();
        }
        instance.reset();
        windowInterface.reset();
    }

    bool tryRender()
    {
        if (!renderContext->beginRender()) return false;

        VkCommandBuffer cmd = renderContext->getActiveCommandBuffer();
        uint32_t frameResourceIndex = renderContext->getActiveFrameResourceIndex();

        // Update UBOs
        nu_vec3_t up = {0.0f, 1.0f, 0.0f};
        nu_vec3_t pos = {0.0f, 0.0f, 0.0f};
        nu_vec3_t eye = {2.0f, 2.0f, 2.0f};

        static float time = 0.0;
        time += nu_context_get_delta_time();
        UniformBufferObject ubo;
        nu_mat4_identity(ubo.model);
        nu_rotate(ubo.model, nu_radian(90.0) * time, up);
        nu_mat4_identity(ubo.view);
        nu_lookat(eye, pos, up, ubo.view);
        nu_mat4_identity(ubo.projection);
        VkExtent2D extent = renderContext->getExtent();
        nu_perspective(nu_radian(90.0f), (float)extent.width / (float)extent.height, 0.1f, 100.0f, ubo.projection);
        std::memcpy(uniformBuffers.at(frameResourceIndex).ptr, &ubo, sizeof(UniformBufferObject));

        // Record command buffer
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());
        VkBuffer vertexBuffers[] = {vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(cmd, indiceBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmd, 6, 1, 0, 0, 0);

        if (!renderContext->endRender()) return false;
        return true;
    }
    void render()
    {
        if (!renderContextOutOfDate) {
            renderContextOutOfDate = !tryRender();
        } else {
            vkDeviceWaitIdle(device->getDevice());
            
            uint32_t width, height;
            nu_window_get_size(&width, &height);
            if (width != 0 && height != 0) {
                destroyRenderContext();
                createRenderContext();

                uint32_t frameResourceCount = renderContext->getFrameResourceCount();
                if (frameResourceCount != previousFrameCount) {
                    destroyFrameResources();
                    createFrameResources();
                    previousFrameCount = frameResourceCount;
                }
                
                renderContextOutOfDate = false;
            }
        }
    }
};

Engine::Engine() : internal(MakeInternalPtr<Internal>()) {}

void Engine::render()
{
    internal->render();
}

void Engine::Interrupt(std::string_view section, std::string_view msg)
{
    nu_interrupt(("[NUVK][" + std::string(section) + "]" + std::string(msg) + "\n").c_str());
}