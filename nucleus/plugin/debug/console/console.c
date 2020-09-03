#include "console.h"

static nu_renderer_font_handle_t _font_handle;
static nu_renderer_label_handle_t _label_handle;

nu_result_t nudebug_plugin_console_initialize(void)
{
    nu_info("INITIALIZE CONSOLE\n");

    nu_renderer_font_create_info_t font_info;
    font_info.filename = "engine/font/Coder's Crux.ttf";
    if (nu_renderer_font_create(&_font_handle, &font_info) != NU_SUCCESS) {
        nu_fatal("Failed to create font.\n");
    }

    nu_renderer_label_create_info_t label_info;
    label_info.x = 50; 
    label_info.y = 50;
    label_info.font = _font_handle;
    label_info.text = "Hello world";
    if (nu_renderer_label_create(&_label_handle, &label_info) != NU_SUCCESS) {
        nu_fatal("Failed to create label.\n");
    }

    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_terminate(void)
{
    nu_info("TERMINATE CONSOLE\n");

    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_console_update(void)
{
    

    return NU_SUCCESS;
}