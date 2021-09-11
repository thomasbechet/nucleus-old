#include <nucleus/core/utility/math/mat3.h>

#include <cglm/cglm.h>

void nu_mat3f_copy(const nu_mat3f_t m, nu_mat3f_t dest)
{
    glm_mat3_copy((vec3*)m, dest);
}
void nu_mat3f_mul(const nu_mat3f_t m0, const nu_mat3f_t m1, nu_mat3f_t dest)
{
    glm_mat3_mul((vec3*)m0, (vec3*)m1, dest);
}
void nu_mat3f_mulv(const nu_mat3f_t m, const nu_vec3f_t v, nu_vec3f_t dest)
{
    glm_mat3_mulv((vec3*)m, (float*)v, dest);
}
void nu_mat3f_identity(nu_mat3f_t m)
{
    glm_mat3_identity((vec3*)m);
}
void nu_mat3f_inverse(nu_mat3f_t m)
{
    glm_mat3_inv((vec3*)m, (vec3*)m);
}
void nu_mat3f_transpose(nu_mat3f_t m)
{
    glm_mat3_transpose((vec3*)m);
}