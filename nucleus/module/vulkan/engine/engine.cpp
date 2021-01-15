#include "engine.hpp"

#include "../utility/logger.hpp"
#include "../vulkan/debugutilsmessenger.hpp"
#include "../vulkan/instance.hpp"
#include "../vulkan/physicaldevice.hpp"
#include "../vulkan/device.hpp"
#include "../vulkan/surface.hpp"
#include "../vulkan/memoryallocator.hpp"
#include "../vulkan/pipeline.hpp"
#include "../vulkan/windowinterface.hpp"
#include "../vulkan/buffer.hpp"
#include "../asset/mesh.hpp"
#include "rendercontext.hpp"

#include <cstring>

using namespace nuvk;

namespace
{
    
}

struct Engine::Internal
{
    // Context
    const bool enableValidationLayers = false; 
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
    std::vector<std::unique_ptr<Mesh>> meshes;

    // Frame Resources
    uint32_t previousFrameCount = 0;

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
            *memoryAllocator,
            renderContext->getRenderPass(),
            renderContext->getExtent(),
            renderContext->getFrameResourceCount()
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
    }

    void destroyFrameResources()
    {
        
    }
    void destroyRenderContext()
    {
        pipeline.reset();
        renderContext.reset();
    }
    void destroyStaticResources()
    {
        for (auto &mesh : meshes) {
            mesh.reset();
        }
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
        nu_vec3f_t up = {0.0f, 1.0f, 0.0f};
        nu_vec3f_t pos = {0.0f, 0.0f, 0.0f};
        nu_vec3f_t eye = {2.0f, 2.0f, 2.0f};

        static int test = 0;
        if (test < 2 || true) {
            static float time = 0.0;
            time += nu_context_get_delta_time();
            UniformBufferObject ubo;
            nu_mat4f_identity(ubo.model);
            nu_rotate(ubo.model, nu_radian(90.0) * time * 0.001, up);
            nu_mat4f_identity(ubo.view);
            nu_lookat(eye, pos, up, ubo.view);
            nu_mat4f_identity(ubo.projection);
            VkExtent2D extent = renderContext->getExtent();
            nu_perspective(nu_radian(70.0f), (float)extent.width / (float)extent.height, 0.1f, 100.0f, ubo.projection);
            pipeline->updateUBO(ubo, frameResourceIndex);
            test += 1;
        }

        // Record command buffer
        VkDescriptorSet descriptorSet = pipeline->getDescriptorSet(frameResourceIndex);
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

        Mesh *mesh = meshes.at(1).get();
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());
        VkBuffer vertexBuffers[] = {mesh->getVertexBuffer().getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
        vkCmdDraw(cmd, mesh->getVertexCount(), 1, 0, 0);

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

void Engine::createMesh(const nu_renderer_mesh_create_info_t &info)
{
    internal->meshes.emplace_back(
        std::make_unique<Mesh>(
            info, 
            *internal->device, 
            *internal->memoryAllocator, 
            *internal->graphicsCommandPool
        )
    );
}

void Engine::Interrupt(std::string_view section, std::string_view msg)
{
    nu_interrupt(("[NUVK][" + std::string(section) + "]" + std::string(msg) + "\n").c_str());
}