#include <nucleus/module/utils/console/console.h>

#include <nucleus/module/utils/console/controller.h>
#include <nucleus/module/utils/console/view.h>

#define NUUTILS_LOGGER_NAME "CONSOLE"

typedef struct {
    nuutils_command_interface_t command_interface;
    bool command_interface_loaded;
    nu_renderer_font_t font;
    nuutils_console_controller_t controller;
    nuutils_console_view_t view;
} nuutils_module_data_t;

static nuutils_module_data_t _module;

static nu_result_t nuutils_console_plugin_on_event(nu_event_id_t id, void *data)
{
    if (id == nu_renderer_viewport_resize_event_get_id()) {
        // nuutils_console_update_position(&_module.console);
    }
    return NU_SUCCESS;
}

nu_result_t nuutils_console_plugin_initialize(void)
{
    /* get command interface */
    _module.command_interface_loaded = false;
    nu_module_t module;
    nu_result_t result = nu_module_get_by_name(NUUTILS_MODULE_NAME, &module);
    if (result == NU_SUCCESS && nu_module_get_interface(module, NUUTILS_COMMAND_INTERFACE_NAME, &_module.command_interface) == NU_SUCCESS) {
        _module.command_interface_loaded = true;
    } else {
        nu_warning(NUUTILS_LOGGER_NAME, "Using console without command plugin.");
    }

    /* subscrive event */
    nu_event_subscribe(nu_renderer_viewport_resize_event_get_id(), nuutils_console_plugin_on_event);

    /* create font */
    nu_renderer_font_create_info_t font_info;
    memset(&font_info, 0, sizeof(nu_renderer_font_create_info_t));
    font_info.filename  = "$ENGINE_DIR/font/Coder's Crux.ttf";
    font_info.font_size = 16;
    if (nu_renderer_font_create(&font_info, &_module.font) != NU_SUCCESS) {
        nu_fatal(NUUTILS_LOGGER_NAME, "Failed to create font.");
    }

    /* create controller */
    nuutils_console_controller_initialize(&_module.controller);
    
    /* create view */
    nuutils_console_view_initialize(&_module.view, _module.font);
    
    return NU_SUCCESS;
}
nu_result_t nuutils_console_plugin_terminate(void)
{
    nuutils_console_view_terminate(&_module.view);
    nuutils_console_controller_terminate(&_module.controller);
    nu_renderer_font_destroy(_module.font);
    
    return NU_SUCCESS;
}
nu_result_t nuutils_console_plugin_update(void)
{
    nuutils_console_controller_update(&_module.controller, &_module.view, 
        _module.command_interface_loaded ? &_module.command_interface : NULL);

    return NU_SUCCESS;
}