#include "renderer.h"

#include "common/logger.h"
#include "glfw/interface.h"
#include "context/context.h"
#include "render/render.h"

nu_result_t nuvk_renderer_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* load glfw interface */
    nu_info(NUVK_VULKAN_LOG_NAME"Loading glfw interface...\n");
    result = nuvk_glfw_load_interface();
    if (result != NU_SUCCESS) return result;

    /* creating context */
    nu_info(NUVK_VULKAN_LOG_NAME"Creating context...\n");
    result = nuvk_context_create();
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

    /* destroy context */
    nuvk_context_destroy();

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_render(void)
{
    nuvk_render_draw();

    return NU_SUCCESS;
}