#include <nucleus/module/vulkan/scene/camera.h>

#include <math.h>

nu_result_t nuvk_camera_initialize(nuvk_camera_t *camera)
{
    camera->fov   = 90.0f;
    camera->ratio = 1.0f;

    nu_vec3f_zero(camera->eye);
    nu_vec3f_copy(NU_VEC3F_FORWARD, camera->center);
    nu_vec3f_copy(NU_VEC3F_UP, camera->up);
    nu_mat4f_identity(camera->vp_matrix);

    nu_vec3f_zero(camera->old_eye);
    nu_vec3f_copy(NU_VEC3F_FORWARD, camera->old_center);
    nu_vec3f_copy(NU_VEC3F_UP, camera->old_up); 
    nu_mat4f_identity(camera->old_vp_matrix);

    return NU_SUCCESS;
}
nu_result_t nuvk_camera_start_frame(
    nuvk_camera_t *camera,
    const nu_vec2u_t viewport_resolution
)
{
    camera->ratio = (float)viewport_resolution[0] / (float)viewport_resolution[1]; 
    camera->focal_length = 1.0f / tanf(camera->fov * 0.5f);
    nu_vec2u_copy(viewport_resolution, camera->resolution);

    nu_mat4f_t view, projection;
    nu_mat4f_lookat(camera->eye, camera->center, camera->up, view);
    nu_mat4f_perspective(camera->fov, camera->ratio, 0.1f, 100.0f, projection);
    nu_mat4f_mul(projection, view, camera->vp_matrix);

    return NU_SUCCESS;
}
nu_result_t nuvk_camera_end_frame(nuvk_camera_t *camera)
{
    nu_vec3f_copy(camera->eye, camera->old_eye);
    nu_vec3f_copy(camera->center, camera->old_center);
    nu_vec3f_copy(camera->up, camera->old_up);
    nu_mat4f_copy(camera->vp_matrix, camera->old_vp_matrix);

    return NU_SUCCESS;
}