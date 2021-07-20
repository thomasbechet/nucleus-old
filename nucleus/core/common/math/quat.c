#include <nucleus/core/common/math/quat.h>

#include <cglm/cglm.h>

void nu_quatf_copy(const nu_quatf_t q, nu_quatf_t dest)
{
    glm_quat_copy((float*)q, dest);
}
void nu_quatf_identity(nu_quatf_t q)
{
    glm_quat_identity((float*)q);
}
void nu_quatf_from_axis(float angle, const nu_vec3f_t axis, nu_quatf_t dest)
{
    glm_quatv(dest, angle, (float*)axis);
}
void nu_quatf_to_mat4(const nu_quatf_t q, nu_mat4f_t dest)
{
    glm_quat_mat4((float*)q, dest);
}
void nu_quatf_to_mat3(const nu_quatf_t q, nu_mat3f_t dest)
{
    glm_quat_mat3((float*)q, dest);
}
void nu_quatf_mul(const nu_quatf_t a, const nu_quatf_t b, nu_quatf_t dest)
{
    glm_quat_mul((float*)a, (float*)b, dest);
}
void nu_quatf_mulv(const nu_quatf_t q, const nu_vec3f_t v, nu_vec3f_t dest)
{
    glm_quat_rotatev((float*)q, (float*)v, dest);
}
void nu_quatf_lerp(const nu_quatf_t a, const nu_quatf_t b, float t, nu_quatf_t dest)
{
    glm_quat_lerp((float*)a, (float*)b, t, dest);
}
void nu_quatf_slerp(const nu_quatf_t a, const nu_quatf_t b, float t, nu_quatf_t dest)
{
    glm_quat_slerp((float*)a, (float*)b, t, dest);
}