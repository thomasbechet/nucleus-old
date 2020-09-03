#include "scene.h"

#include "render.h"

#define MAX_STATICMESH_COUNT 1024

typedef struct {
    nusr_camera_t camera;
    uint32_t staticmesh_count;
    nusr_staticmesh_t *staticmeshes;
} nusr_scene_data_t;

static nusr_scene_data_t _data;

nu_result_t nusr_scene_initialize(void)
{
    /* camera */
    nu_vec3_zero(_data.camera.eye);
    nu_vec3_copy(_data.camera.center, (nu_vec3_t){0, 0, -1});
    nu_vec3_copy(_data.camera.up, (nu_vec3_t){0, 1, 0});
    _data.camera.fov = 90.0f;
    _data.camera.near = 0.1f;
    _data.camera.far = 1000.0f;

    /* staticmesh */
    _data.staticmesh_count = 0;
    _data.staticmeshes = (nusr_staticmesh_t*)nu_malloc(sizeof(nusr_staticmesh_t) * MAX_STATICMESH_COUNT);
    for (uint32_t i = 0; i < MAX_STATICMESH_COUNT; i++) {
        _data.staticmeshes[i].active = false;
    }

    return NU_SUCCESS;
}
nu_result_t nusr_scene_terminate(void)
{
    nu_free(_data.staticmeshes);

    return NU_SUCCESS;
}
nu_result_t nusr_scene_render(nusr_framebuffer_t *color_buffer, nusr_framebuffer_t *depth_buffer)
{
    nusr_scene_render_global(
        color_buffer, depth_buffer,
        &_data.camera,
        _data.staticmeshes, _data.staticmesh_count
    );

    return NU_SUCCESS;
}

nu_result_t nusr_scene_camera_set_fov(nu_renderer_camera_handle_t handle, float fov)
{
    _data.camera.fov = fov;
    return NU_SUCCESS;
}
nu_result_t nusr_scene_camera_set_eye(nu_renderer_camera_handle_t handle, const nu_vec3_t eye)
{
    nu_vec3_copy(eye, _data.camera.eye);
    return NU_SUCCESS;
}
nu_result_t nusr_scene_camera_set_center(nu_renderer_camera_handle_t handle, const nu_vec3_t center)
{
    nu_vec3_copy(center, _data.camera.center);
    return NU_SUCCESS;
}
nu_result_t nusr_scene_camera_set_transform(const nu_mat4_t transform)
{
    // vec3 temp;
    // glm_vec3_copy(temp, (vec3){0, 0, -1});
    // glm_mat4_mulv3(transform, temp, 0, temp);
    nu_fatal("not implemented.\n");
    return NU_SUCCESS;
}

nu_result_t nusr_scene_staticmesh_create(nu_renderer_staticmesh_handle_t *handle, const nu_renderer_staticmesh_create_info_t *info)
{
    uint32_t found_id = MAX_STATICMESH_COUNT;
    for (uint32_t i = 0; i < _data.staticmesh_count; i++) {
        if (!_data.staticmeshes[i].active) {
            found_id = i;
            break;
        }
    }

    if (found_id == MAX_STATICMESH_COUNT) {
        if (_data.staticmesh_count >= MAX_STATICMESH_COUNT) return NU_FAILURE;
        found_id = _data.staticmesh_count;
        _data.staticmesh_count++;
    }

    _data.staticmeshes[found_id].active = true;
    _data.staticmeshes[found_id].mesh = (uint64_t)info->mesh;
    _data.staticmeshes[found_id].texture = (uint64_t)info->texture;
    nu_mat4_copy(info->transform, _data.staticmeshes[found_id].transform);

    *((uint32_t*)handle) = found_id;

    return NU_SUCCESS;
}
nu_result_t nusr_scene_staticmesh_destroy(nu_renderer_staticmesh_handle_t handle)
{
    uint32_t id = *((uint32_t*)handle);

    if (!_data.staticmeshes[id].active) return NU_FAILURE;

    _data.staticmeshes[id].active = false;

    return NU_SUCCESS;
}
nu_result_t nusr_scene_staticmesh_set_transform(nu_renderer_staticmesh_handle_t handle, const nu_mat4_t m)
{
    uint32_t id = *((uint32_t*)handle);

    if (!_data.staticmeshes[id].active) return NU_FAILURE;

    nu_mat4_copy(m, _data.staticmeshes[id].transform);

    return NU_SUCCESS;
}