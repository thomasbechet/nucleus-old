#include <nucleus/core/common/math/mat4.h>

#include <cglm/cglm.h>

void nu_mat4f_copy(const nu_mat4f_t m, nu_mat4f_t dest)
{
    glm_mat4_copy((vec4*)m, dest);
}
void nu_mat4f_mul(const nu_mat4f_t m0, const nu_mat4f_t m1, nu_mat4f_t dest)
{
    glm_mat4_mul((vec4*)m0, (vec4*)m1, dest);
}
void nu_mat4f_mulv(const nu_mat4f_t m, const nu_vec4f_t v, nu_vec4f_t dest)
{
    glm_mat4_mulv((vec4*)m, (float*)v, dest);
}
void nu_mat4f_mulv3(const nu_mat4f_t m, const nu_vec3f_t v, float w, nu_vec3f_t dest)
{
    glm_mat4_mulv3((vec4*)m, (float*)v, w, (float*)dest);
}
void nu_mat4f_identity(nu_mat4f_t m)
{
    glm_mat4_identity((vec4*)m);
}
void nu_mat4f_translate(nu_mat4f_t m, const nu_vec3f_t translate)
{
    glm_translate(m, (float*)translate);
}
void nu_mat4f_rotate(nu_mat4f_t m, const nu_quatf_t rotation)
{
    glm_quat_rotate(m, (float*)rotation, m);
}
void nu_mat4f_rotate_axis(nu_mat4f_t m, float angle, const nu_vec3f_t axis)
{
    glm_rotate(m, angle, (float*)axis);
}
void nu_mat4f_scale(nu_mat4f_t m, const nu_vec3f_t scale)
{
    glm_scale(m, (float*)scale);
}
void nu_mat4f_pick_mat3(const nu_mat4f_t m, nu_mat3f_t dest)
{
    glm_mat4_pick3t((vec4*)m, dest);
}
void nu_mat4f_inv(const nu_mat4f_t m, nu_mat4f_t dest)
{
    glm_mat4_inv((vec4*)m, dest);
}