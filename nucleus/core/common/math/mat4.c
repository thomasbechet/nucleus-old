#include "mat4.h"

#include <cglm/cglm.h>

void nu_mat4_copy(const nu_mat4_t m, nu_mat4_t dest)
{
    glm_mat4_copy((vec4*)m, dest);
}
void nu_mat4_mul(const nu_mat4_t m0, const nu_mat4_t m1, nu_mat4_t dest)
{
    glm_mat4_mul((vec4*)m0, (vec4*)m1, dest);
}
void nu_mat4_mulv(const nu_mat4_t m, const nu_vec4_t v, nu_vec4_t dest)
{
    glm_mat4_mulv((vec4*)m, (float*)v, dest);
}
void nu_mat4_mulv3(const nu_mat4_t m, const nu_vec3_t v, float w, nu_vec3_t dest)
{
    glm_mat4_mulv3((vec4*)m, (float*)v, w, (float*)dest);
}
void nu_mat4_identity(nu_mat4_t m)
{
    glm_mat4_identity((vec4*)m);
}
void nu_translate(nu_mat4_t m, const nu_vec3_t translate)
{
    glm_translate(m, (float*)translate);
}
void nu_rotate(nu_mat4_t m, float angle, const nu_vec3_t axis)
{
    glm_rotate(m, angle, (float*)axis);
}
void nu_scale(nu_mat4_t m, const nu_vec3_t scale)
{
    glm_scale(m, (float*)scale);
}