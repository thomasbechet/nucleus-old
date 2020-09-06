#include "renderer.h"

#include "../context/config.h"
#include "../../system/softrast/module/interface.h"
#include "../../system/vulkan/module/interface.h"

#define NU_LOGGER_RENDERER_NAME "[RENDERER] "

static const char *nu_renderer_api_names[] = {
    "engine/system/nucleus-renderer-none",
    "engine/system/"NUSR_MODULE_NAME,
    "engine/system/"NUVK_MODULE_NAME,
    "engine/system/nucleus-opengl"
};

typedef struct {
    nu_module_handle_t module;
    nu_renderer_interface_t interface;
} nu_system_renderer_t;

static nu_system_renderer_t _system;

nu_result_t nu_system_renderer_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_system, 0, sizeof(nu_system_renderer_t));
    nu_renderer_api_t api = nu_config_get().renderer.api;

    /* load renderer module */
    result = nu_module_load(&_system.module, nu_renderer_api_names[api]);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load renderer interface accessor */
    nu_renderer_interface_loader_pfn_t load_interface;
    result = nu_module_load_function(_system.module, NU_RENDERER_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_RENDERER_NAME"Failed to load renderer loader.\n");
        return result;
    }

    /* load renderer interface */
    result = load_interface(&_system.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_RENDERER_NAME"Failed to load interface.\n");
        return result;
    }

    /* initialize renderer system */
    result = _system.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_RENDERER_NAME"Failed to initialize renderer system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_system_renderer_terminate(void)
{
    /* terminate renderer system */
    _system.interface.terminate();

    return NU_SUCCESS;
}
nu_result_t nu_system_renderer_render(void)
{
    _system.interface.render();
    return NU_SUCCESS;
}

nu_module_handle_t nu_system_renderer_get_module_id(void)
{
    return _system.module;
}

nu_result_t nu_renderer_mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info)
{
    return _system.interface.mesh_create(handle, info);
}
nu_result_t nu_renderer_mesh_destroy(nu_renderer_mesh_handle_t handle)
{
    return _system.interface.mesh_destroy(handle);
}

nu_result_t nu_renderer_texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info)
{
    return _system.interface.texture_create(handle, info);
}
nu_result_t nu_renderer_texture_destroy(nu_renderer_texture_handle_t handle)
{
    return _system.interface.texture_destroy(handle);
}

nu_result_t nu_renderer_font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info)
{
    return _system.interface.font_create(handle, info);
}
nu_result_t nu_renderer_font_destroy(nu_renderer_font_handle_t handle)
{
    return _system.interface.font_destroy(handle);
}

nu_result_t nu_renderer_camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info)
{
    return _system.interface.camera_create(handle, info);
}
nu_result_t nu_renderer_camera_destroy(nu_renderer_camera_handle_t handle)
{
    return _system.interface.camera_destroy(handle);
}
nu_result_t nu_renderer_camera_set_fov(nu_renderer_camera_handle_t handle, float fov)
{
    return _system.interface.camera_set_fov(handle, fov);
}
nu_result_t nu_renderer_camera_set_eye(nu_renderer_camera_handle_t handle, const nu_vec3_t eye)
{
    return _system.interface.camera_set_eye(handle, eye);
}
nu_result_t nu_renderer_camera_set_center(nu_renderer_camera_handle_t handle, const nu_vec3_t center)
{
    return _system.interface.camera_set_center(handle, center);
}

nu_result_t nu_renderer_staticmesh_create(nu_renderer_staticmesh_handle_t *handle, const nu_renderer_staticmesh_create_info_t *info)
{
    return _system.interface.staticmesh_create(handle, info);
}
nu_result_t nu_renderer_staticmesh_destroy(nu_renderer_staticmesh_handle_t handle)
{
    return _system.interface.staticmesh_destroy(handle);
}
nu_result_t nu_renderer_staticmesh_set_transform(nu_renderer_staticmesh_handle_t handle, const nu_mat4_t transform)
{
    return _system.interface.staticmesh_set_transform(handle, transform);
}

nu_result_t nu_renderer_label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info)
{
    return _system.interface.label_create(handle, info);
}
nu_result_t nu_renderer_label_destroy(nu_renderer_label_handle_t handle)
{
    return _system.interface.label_destroy(handle);
}
nu_result_t nu_renderer_label_set_position(nu_renderer_label_handle_t handle, int32_t x, int32_t y)
{
    return _system.interface.label_set_position(handle, x, y);
}
nu_result_t nu_renderer_label_set_text(nu_renderer_label_handle_t handle, const char *text)
{
    return _system.interface.label_set_text(handle, text);
}