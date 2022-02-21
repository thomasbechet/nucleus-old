#include <nucleus/core/system/renderer/renderer.h>

#include <nucleus/core/config/interface.h>
#include <nucleus/core/coresystem/logger/interface.h>
#include <nucleus/module/vulkan/module/interface.h>

#define NU_LOGGER_RENDERER_NAME "RENDERER"

static const char *nu_renderer_api_names[] = {
    "$MODULE/nucleus-renderer-none",
    "$MODULE/nucleus-renderer-none",
    "$MODULE/"NUVK_MODULE_NAME
};

typedef struct {
    nu_module_t module;
    nu_renderer_interface_t interface;
    nu_event_id_t viewport_resize_event_id;
} nu_system_data_t;

static nu_system_data_t _system;

static nu_result_t initialize_event(void)
{
    nu_result_t result;
    nu_event_register_info_t info;

    info.initialize = NULL;
    info.terminate  = NULL;
    info.size       = sizeof(nu_renderer_viewport_resize_event_t);
    result = nu_event_register(&info, &_system.viewport_resize_event_id);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NU_LOGGER_RENDERER_NAME, "Failed to register viewport resize event.");

    return NU_SUCCESS;
}

nu_result_t nu_renderer_initialize(void)
{
    nu_result_t result;

    memset(&_system, 0, sizeof(nu_system_data_t));
    nu_renderer_api_t api = nu_config_get().renderer.api;

    /* initialize event */
    result = initialize_event();
    NU_CHECK(result == NU_SUCCESS, return result, NU_LOGGER_RENDERER_NAME, "Failed to initialize event.");

    if (api != NU_RENDERER_API_NONE) {
        /* get renderer module */
        result = nu_module_load(nu_renderer_api_names[api], &_system.module);
        NU_CHECK(result == NU_SUCCESS, return result, NU_LOGGER_RENDERER_NAME, "Failed to get module.");

        /* get renderer interface */
        result = nu_module_get_interface(_system.module, NU_RENDERER_INTERFACE_NAME, &_system.interface);
        NU_CHECK(result == NU_SUCCESS, return result, NU_LOGGER_RENDERER_NAME, "Failed to get interface.");

        /* initialize renderer system */
        if (_system.interface.initialize) {
            result = _system.interface.initialize();
            NU_CHECK(result == NU_SUCCESS, return result, NU_LOGGER_RENDERER_NAME, "Failed to initialize renderer system.");
        }
    } else {
        nu_info(NU_LOGGER_RENDERER_NAME, "Running in passive mode.");
    }

    return NU_SUCCESS;
}
nu_result_t nu_renderer_terminate(void)
{
    if (_system.interface.terminate) {
        return _system.interface.terminate();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_renderer_start(void)
{
    if (_system.interface.start) {
        return _system.interface.start();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_renderer_stop(void)
{
    if (_system.interface.stop) {
        return _system.interface.stop();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_renderer_render(void)
{
    if (_system.interface.render) {
        return _system.interface.render();
    } else {
        return NU_SUCCESS;
    }
}

nu_module_t nu_renderer_get_module(void)
{
    return _system.module;
}

nu_result_t nu_renderer_camera_create(const nu_renderer_camera_create_info_t *info, nu_renderer_camera_t *handle)
{
    if (_system.interface.camera_create) {
        return _system.interface.camera_create(info, handle);
    } else {
        *handle = NU_NULL_HANDLE;
        return NU_SUCCESS;
    }
}
nu_result_t nu_renderer_camera_destroy(nu_renderer_camera_t handle)
{
    if (_system.interface.camera_destroy) {
        return _system.interface.camera_destroy(handle);
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_renderer_camera_set_fov(nu_renderer_camera_t handle, float fov)
{
    if (_system.interface.camera_set_fov) {
        return _system.interface.camera_set_fov(handle, fov);
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_renderer_camera_set_view(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    if (_system.interface.camera_set_view) {
        return _system.interface.camera_set_view(handle, eye, forward, up);
    } else {
        return NU_SUCCESS;
    }
}

nu_result_t nu_renderer_viewport_set_size(const nu_vec2u_t size)
{
    nu_renderer_viewport_resize_event_t event;
    nu_vec2u_copy(size, event.size);
    nu_renderer_viewport_get_size(event.old_size);

    nu_result_t result = NU_SUCCESS;
    if (_system.interface.viewport_set_size) {
        result = _system.interface.viewport_set_size(size);
    } else {
        return NU_SUCCESS;
    }

    nu_event_post(_system.viewport_resize_event_id, &event);

    return result;
}
nu_result_t nu_renderer_viewport_get_size(nu_vec2u_t size)
{
    if (_system.interface.viewport_get_size) {
        return _system.interface.viewport_get_size(size);
    } else {
        size[0] = size[1] = 0;
        return NU_SUCCESS;
    }
}

nu_event_id_t nu_renderer_viewport_resize_event_get_id(void)
{
    return _system.viewport_resize_event_id;
}