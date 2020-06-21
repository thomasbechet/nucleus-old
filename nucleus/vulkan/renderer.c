#include "renderer.h"

#include "common/logger.h"
#include "glfw/interface.h"
#include "context/context.h"
#include "render/render_pass.h"
#include "render/graphics_pipeline.h"
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

    /* creating context */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating context...\n");
    result = nuvk_context_create();
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

    /* destroy context */
    nuvk_context_destroy();

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_render(void)
{
    nuvk_render_draw();

    return NU_SUCCESS;
}