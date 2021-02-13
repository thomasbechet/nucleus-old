#include <nucleus/core/system/renderer/renderer.h>

#include <nucleus/core/config/interface.h>

#include <nucleus/module/softrast.h>

#define NU_LOGGER_RENDERER_NAME "[RENDERER] "

static const char *nu_renderer_api_names[] = {
    "engine/module/nucleus-renderer-none",
    "engine/module/"NUSR_MODULE_NAME,
    "engine/module/nucleus-renderer-none",
    "engine/module/nucleus-renderer-none",
    "engine/module/nucleus-renderer-none"
};

typedef struct {
    nu_module_handle_t module;
    nu_renderer_interface_t interface;
    nu_event_id_t viewport_resize_event_id;
} nu_renderer_data_t;

static nu_renderer_data_t _data;

static nu_result_t initialize_event(void)
{
    nu_event_register_info_t info;

    info.initialize = NULL;
    info.terminate  = NULL;
    info.size       = sizeof(nu_renderer_viewport_resize_event_t);
    if (nu_event_register(&_data.viewport_resize_event_id, &info) != NU_SUCCESS) {
        nu_warning(NU_LOGGER_RENDERER_NAME"Failed to register viewport resize event.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

nu_result_t nu_renderer_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_data, 0, sizeof(nu_renderer_data_t));
    nu_renderer_api_t api = nu_config_get().renderer.api;

    /* initialize event */
    result = initialize_event();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_RENDERER_NAME"Failed to initialize event.\n");
        return result;
    }

    /* get renderer module */
    result = nu_module_load(&_data.module, nu_renderer_api_names[api]);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load renderer interface */
    result = nu_module_load_interface(_data.module, NU_RENDERER_INTERFACE_NAME, &_data.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_RENDERER_NAME"Failed to load interface.\n");
        return result;
    }

    /* initialize renderer system */
    result = _data.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_RENDERER_NAME"Failed to initialize renderer system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_renderer_terminate(void)
{
    return _data.interface.terminate();
}
nu_result_t nu_renderer_start(void)
{
    return _data.interface.start();
}
nu_result_t nu_renderer_stop(void)
{
    return _data.interface.stop();
}
nu_result_t nu_renderer_render(void)
{
    return _data.interface.render();
}

nu_module_handle_t nu_renderer_get_module_handle(void)
{
    return _data.module;
}

nu_result_t nu_renderer_mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info)
{
    return _data.interface.mesh_create(handle, info);
}
nu_result_t nu_renderer_mesh_destroy(nu_renderer_mesh_handle_t handle)
{
    return _data.interface.mesh_destroy(handle);
}

nu_result_t nu_renderer_texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info)
{
    return _data.interface.texture_create(handle, info);
}
nu_result_t nu_renderer_texture_destroy(nu_renderer_texture_handle_t handle)
{
    return _data.interface.texture_destroy(handle);
}

nu_result_t nu_renderer_material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info)
{
    return _data.interface.material_create(handle, info);
}
nu_result_t nu_renderer_material_destroy(nu_renderer_material_handle_t handle)
{
    return _data.interface.material_destroy(handle);
}

nu_result_t nu_renderer_font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info)
{
    return _data.interface.font_create(handle, info);
}
nu_result_t nu_renderer_font_destroy(nu_renderer_font_handle_t handle)
{
    return _data.interface.font_destroy(handle);
}
nu_result_t nu_renderer_font_get_text_size(nu_renderer_font_handle_t handle, const char *text, nu_vec2u_t size)
{
    return _data.interface.font_get_text_size(handle, text, size);
}

nu_result_t nu_renderer_camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info)
{
    return _data.interface.camera_create(handle, info);
}
nu_result_t nu_renderer_camera_destroy(nu_renderer_camera_handle_t handle)
{
    return _data.interface.camera_destroy(handle);
}
nu_result_t nu_renderer_camera_set_fov(nu_renderer_camera_handle_t handle, float fov)
{
    return _data.interface.camera_set_fov(handle, fov);
}
nu_result_t nu_renderer_camera_set_view(nu_renderer_camera_handle_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    return _data.interface.camera_set_view(handle, eye, forward, up);
}

nu_result_t nu_renderer_model_create(nu_renderer_model_handle_t *handle, const nu_renderer_model_create_info_t *info)
{
    return _data.interface.model_create(handle, info);
}
nu_result_t nu_renderer_model_destroy(nu_renderer_model_handle_t handle)
{
    return _data.interface.model_destroy(handle);
}
nu_result_t nu_renderer_model_set_transform(nu_renderer_model_handle_t handle, const nu_mat4f_t transform)
{
    return _data.interface.model_set_transform(handle, transform);
}

nu_result_t nu_renderer_label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info)
{
    return _data.interface.label_create(handle, info);
}
nu_result_t nu_renderer_label_destroy(nu_renderer_label_handle_t handle)
{
    return _data.interface.label_destroy(handle);
}
nu_result_t nu_renderer_label_set_position(nu_renderer_label_handle_t handle, const nu_vec2i_t position)
{
    return _data.interface.label_set_position(handle, position);
}
nu_result_t nu_renderer_label_set_text(nu_renderer_label_handle_t handle, const char *text)
{
    return _data.interface.label_set_text(handle, text);
}

nu_result_t nu_renderer_rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info)
{
    return _data.interface.rectangle_create(handle, info);
}
nu_result_t nu_renderer_rectangle_destroy(nu_renderer_rectangle_handle_t handle)
{
    return _data.interface.rectangle_destroy(handle);
}
nu_result_t nu_renderer_rectangle_set_rect(nu_renderer_rectangle_handle_t handle, nu_rect_t rect)
{
    return _data.interface.rectangle_set_rect(handle, rect);
}

nu_result_t nu_renderer_viewport_set_size(const nu_vec2u_t size)
{
    nu_renderer_viewport_resize_event_t event;
    nu_vec2u_copy(size, event.size);
    nu_renderer_viewport_get_size(event.old_size);

    nu_result_t result;
    result = _data.interface.viewport_set_size(size);
    if (result == NU_SUCCESS) {
        nu_event_post(_data.viewport_resize_event_id, &event);
    }
    return result;
}
nu_result_t nu_renderer_viewport_get_size(nu_vec2u_t size)
{
    return _data.interface.viewport_get_size(size);
}

nu_event_id_t nu_renderer_viewport_resize_event_get_id(void)
{
    return _data.viewport_resize_event_id;
}