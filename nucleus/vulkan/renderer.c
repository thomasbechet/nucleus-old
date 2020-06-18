#include "renderer.h"

#include "common/logger.h"
#include "glfw/interface.h"
#include "presentation/instance.h"
#include "presentation/debug_messenger.h"
#include "presentation/surface.h"
#include "presentation/physical_device.h"
#include "presentation/device.h"
#include "presentation/swapchain.h"
#include "pipeline/render_pass.h"
#include "pipeline/graphics_pipeline.h"
#include "render/framebuffer.h"
#include "render/command.h"
#include "render/render.h"

typedef struct {
    uint32_t current_frame;
} nuvk_renderer_data_t;

static nuvk_renderer_data_t _data;

nu_result_t nuvk_renderer_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* data initialization */
    _data.current_frame = 0;

    /* load glfw interface */
    nu_info(NUVK_VULKAN_LOG_NAME"Loading glfw interface...\n");
    result = nuvk_glfw_load_interface();
    if (result != NU_SUCCESS) return result;

    /* create instance */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating instance...\n");
    result = nuvk_instance_create();
    if (result != NU_SUCCESS) return result;

    /* create debug messenger */
#ifdef NU_DEBUG
    nu_info(NUVK_VULKAN_LOG_NAME"Creating debug messenger...\n");
    result = nuvk_debug_messenger_create(); 
    if (result != NU_SUCCESS) return result;
#endif

    /* create surface */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating surface...\n");
    result = nuvk_surface_create();
    if (result != NU_SUCCESS) return result;

    /* pick physical device */
    nu_info(NUVK_VULKAN_LOG_NAME"Picking physical device...\n");
    result = nuvk_physical_device_pick();
    if (result != NU_SUCCESS) return result;

    /* create device */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating device...\n");
    result = nuvk_device_create();
    if (result != NU_SUCCESS) return result;

    /* create swap chain */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating swapchain...\n");
    result = nuvk_swapchain_create();
    if (result != NU_SUCCESS) return result;

    /* create render pass */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating render pass...\n");
    result = nuvk_render_pass_create();
    if (result != NU_SUCCESS) return result;

    /* create graphics pipeline */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating graphics pipeline...\n");
    result = nuvk_graphics_pipeline_create();
    if (result != NU_SUCCESS) return result;

    /* create framebuffers */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating framebuffers...\n");
    result = nuvk_framebuffer_create();
    if (result != NU_SUCCESS) return result;

    /* create command pool */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating command pool...\n");
    result = nuvk_command_pool_create();
    if (result != NU_SUCCESS) return result;

    /* create command buffers */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating command buffers...\n");
    result = nuvk_command_buffers_create();
    if (result != NU_SUCCESS) return result;

    /* create render */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating render...\n");
    result = nuvk_render_create();
    if (result != NU_SUCCESS) return result;

    return result;
}
nu_result_t nuvk_renderer_terminate(void)
{
    /* destroy render */
    nuvk_render_destroy();

    /* destroy command buffers */
    nuvk_command_buffers_destroy();

    /* destroy command pool */
    nuvk_command_pool_destroy();

    /* destroy framebuffers */
    nuvk_framebuffer_destroy();

    /* destroy graphics pipeline */
    nuvk_graphics_pipeline_destroy();

    /* destroy render pass */
    nuvk_render_pass_destroy();

    /* destroy swapchain */
    nuvk_swapchain_destroy();

    /* destroy device */
    nuvk_device_destroy();

    /* destroy surface */
    nuvk_surface_destroy();

    /* destroy debug messenger */
#ifdef NU_DEBUG
    nuvk_debug_messenger_destroy();
#endif

    /* destroy instance */
    nuvk_instance_destroy();

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_render(void)
{
    nuvk_render_draw();

    return NU_SUCCESS;
}