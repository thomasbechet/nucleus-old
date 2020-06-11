#include "config.h"

nu_result_t nu_config_default(nu_config_t *config)
{
    config->window_api = NU_WINDOW_API_GLFW;
    config->renderer_api = NU_RENDERER_API_VULKAN;

    return NU_SUCCESS;
}