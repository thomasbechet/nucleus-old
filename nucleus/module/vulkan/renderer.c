#include <nucleus/module/vulkan/renderer.h>

#include <nucleus/module/vulkan/common/logger.h>
#include <nucleus/module/vulkan/glfw/interface.h>
#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/core/memory.h>
#include <nucleus/module/vulkan/core/command_pool.h>
#include <nucleus/module/vulkan/core/swapchain.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/sdf/renderer.h>

typedef struct {
    nuvk_context_t context;
    nuvk_memory_manager_t memory_manager;
    nuvk_shader_manager_t shader_manager;
    nuvk_command_pool_t command_pool;
    nuvk_swapchain_t swapchain;
    nuvk_render_context_t render_context;

    nuvk_sdf_renderer_t sdf;

    bool render_context_out_of_date;
} nuvk_module_data_t;

static nuvk_module_data_t _module;

nu_result_t nuvk_renderer_initialize(void)
{
    _module.render_context_out_of_date = false;

    nu_result_t result = nuvk_glfw_get_interface();
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to get glfw interface.");

    if (nuvk_context_initialize(&_module.context) != NU_SUCCESS) return NU_FAILURE;
    if (nuvk_memory_manager_initialize(&_module.memory_manager, &_module.context) != NU_SUCCESS) return NU_FAILURE;
    if (nuvk_shader_manager_initialize(&_module.shader_manager) != NU_SUCCESS) return NU_FAILURE;
    if (nuvk_command_pool_initialize(&_module.command_pool, &_module.context) != NU_SUCCESS) return NU_FAILURE;
    if (nuvk_swapchain_initialize(&_module.swapchain, &_module.context, UINT32_MAX, UINT32_MAX) != NU_SUCCESS) return NU_FAILURE;
    if (nuvk_render_context_initialize(&_module.render_context, &_module.context, &_module.swapchain, &_module.command_pool, 3) != NU_SUCCESS) return NU_FAILURE;

    if (nuvk_sdf_renderer_initialize(&_module.sdf, &_module.context, &_module.memory_manager, 
        &_module.shader_manager, &_module.swapchain, &_module.render_context) != NU_SUCCESS) return NU_FAILURE;

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_terminate(void)
{
    nuvk_sdf_renderer_terminate(&_module.sdf, &_module.context, &_module.memory_manager);

    nuvk_render_context_terminate(&_module.render_context, &_module.context);
    nuvk_swapchain_terminate(&_module.swapchain, &_module.context);
    nuvk_command_pool_terminate(&_module.command_pool, &_module.context);
    nuvk_shader_manager_terminate(&_module.shader_manager);
    nuvk_memory_manager_terminate(&_module.memory_manager);
    nuvk_context_terminate(&_module.context);

    return NU_SUCCESS;
}

static bool nuvk_renderer_try_render(void)
{
    if (!nuvk_render_context_begin(&_module.render_context, &_module.context, &_module.swapchain)) return false;

    nuvk_sdf_renderer_render(&_module.sdf, &_module.context, &_module.swapchain, &_module.render_context);

    if (!nuvk_render_context_end(&_module.render_context, &_module.context, &_module.swapchain)) return false;

    return true;
}
nu_result_t nuvk_renderer_render(void)
{
    if (!_module.render_context_out_of_date) {
        _module.render_context_out_of_date = !nuvk_renderer_try_render();
    } else {
        vkDeviceWaitIdle(_module.context.device);
        nu_vec2u_t size;
        nu_window_get_size(size);
        if (size[0] != 0 && size[1] != 0) {
            nu_info(NUVK_LOGGER_NAME, "Swapchain updated (%ldx%ld) with %ld frame(s).", size[0], size[1], _module.swapchain.image_count);

            /* recreate swapchain */
            nuvk_swapchain_recreate(&_module.swapchain, &_module.context, size[0], size[1]);

            /* notify renderer */
            nuvk_sdf_renderer_update_swapchain(&_module.sdf, &_module.context, &_module.memory_manager, &_module.swapchain);

            /* reset out of date flag */
            _module.render_context_out_of_date = false;
        }
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_renderer_camera_set_fov(nu_renderer_camera_t handle, float fov)
{
    _module.sdf.scene.camera.fov = fov;

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_camera_set_view(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    nu_vec3f_copy(eye, _module.sdf.scene.camera.eye);
    nu_vec3f_copy(eye, _module.sdf.scene.camera.center);
    nu_vec3f_add(_module.sdf.scene.camera.center, forward, _module.sdf.scene.camera.center);
    nu_vec3f_copy(up, _module.sdf.scene.camera.up);

    return NU_SUCCESS;
}

nu_result_t nuvk_renderer_viewport_set_size(const nu_vec2u_t size)
{
    return nuvk_sdf_renderer_update_viewport_size(&_module.sdf, &_module.context, &_module.memory_manager, size);
}

nu_result_t nuvk_sdf_instance_type_register(const nuvk_sdf_instance_type_info_t *info, nuvk_sdf_instance_type_t *handle)
{
    return nuvk_sdf_renderer_register_instance_type(&_module.sdf, &_module.context, &_module.shader_manager, info, handle);
}
nu_result_t nuvk_sdf_instance_create(const nuvk_sdf_instance_info_t *info, nuvk_sdf_instance_t *handle)
{
    return nuvk_sdf_scene_create_instance(&_module.sdf.scene, &_module.render_context, info, handle);
}
nu_result_t nuvk_sdf_instance_destroy(nuvk_sdf_instance_t handle)
{
    return nuvk_sdf_scene_destroy_instance(&_module.sdf.scene, &_module.render_context, handle);
}
nu_result_t nuvk_sdf_instance_update_transform(nuvk_sdf_instance_t handle, const nu_transform_t *transform)
{
    return nuvk_sdf_scene_update_instance_transform(&_module.sdf.scene, &_module.render_context, handle, transform);
}
nu_result_t nuvk_sdf_instance_update_data(nuvk_sdf_instance_t handle, const void *data)
{
    return nuvk_sdf_scene_update_instance_data(&_module.sdf.scene, &_module.render_context, handle, data);
}