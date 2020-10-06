#include "softrast.h"

#include "common/logger.h"
#include "common/config.h"
#include "viewport/viewport.h"
#include "scene/scene.h"
#include "gui/gui.h"
#include "asset/mesh.h"
#include "asset/texture.h"
#include "asset/material.h"
#include "asset/font.h"

#include "../glfw/module/interface.h"

typedef struct {
    nuglfw_window_interface_t glfw_interface;
} nusr_data_t;

static nusr_data_t _data;

static nu_result_t load_glfw_interface(void)
{
    nu_module_handle_t window_module = nu_system_window_get_module_handle();
    if (nu_module_get_id(window_module) != NUGLFW_MODULE_ID) {
        nu_fatal(NUSR_LOGGER_NAME"Software rasterizer requires GLFW module to work.\n");
        return NU_FAILURE;
    }

    nuglfw_window_interface_loader_pfn_t load_interface;
    if (nu_module_load_function(window_module, NUGLFW_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
        nu_warning(NUSR_LOGGER_NAME"Software rasterizer failed to load glfw loader.\n");
        return NU_FAILURE;
    }

    if (load_interface(&_data.glfw_interface) != NU_SUCCESS) {
        nu_warning(NUSR_LOGGER_NAME"Software rasterizer failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

nu_result_t nusr_initialize(void)
{
    /* loading glfw interface */
    nu_info(NUSR_LOGGER_NAME"Loading glfw interface...\n");
    if (load_glfw_interface() != NU_SUCCESS) {
        nu_warning(NUSR_LOGGER_NAME"Failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    /* initialize assets */
    nu_info(NUSR_LOGGER_NAME"Initializing assets...\n");
    if (nusr_mesh_initialize() != NU_SUCCESS) return NU_FAILURE;
    if (nusr_texture_initialize() != NU_SUCCESS) return NU_FAILURE;
    if (nusr_material_initialize() != NU_SUCCESS) return NU_FAILURE;
    if (nusr_font_initialize() != NU_SUCCESS) return NU_FAILURE;

    /* initialize viewport */
    nu_info(NUSR_LOGGER_NAME"Initializing viewport...\n");
    nusr_viewport_initialize();

    /* initialize scene */
    nu_info(NUSR_LOGGER_NAME"Intializing scene...\n");
    nusr_scene_initialize();

    /* initialize gui */
    nu_info(NUSR_LOGGER_NAME"Initialize gui...\n");
    nusr_gui_initialize();

    return NU_SUCCESS;
}
nu_result_t nusr_terminate(void)
{
    /* terminate gui */
    nu_info(NUSR_LOGGER_NAME"Terminating gui...\n");
    nusr_gui_terminate();

    /* terminate scene */
    nu_info(NUSR_LOGGER_NAME"Terminating scene...\n");
    nusr_scene_terminate();

    /* terminate viewport */
    nu_info(NUSR_LOGGER_NAME"Terminating viewport...\n");
    nusr_viewport_terminate();

    /* terminate assets */
    nu_info(NUSR_LOGGER_NAME"Terminating assets...\n");
    nusr_font_terminate();
    nusr_material_terminate();
    nusr_texture_terminate();
    nusr_mesh_terminate();

    return NU_SUCCESS;
}
nu_result_t nusr_render(void)
{
    nusr_renderbuffer_t *renderbuffer;
    nusr_viewport_get_renderbuffer(&renderbuffer);

    nusr_scene_render(renderbuffer);
    nusr_gui_render(&renderbuffer->color_buffer);
    _data.glfw_interface.present_surface(
        renderbuffer->color_buffer.width, 
        renderbuffer->color_buffer.height,
        renderbuffer->color_buffer.pixels
    );

    return NU_SUCCESS;
}
