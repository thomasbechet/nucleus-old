#include <nucleus/core/common/math/vec3.h>

#include <cglm/cglm.h>

void nu_vec3f_copy(const nu_vec3f_t v, nu_vec3f_t dest)
{
    glm_vec3_copy((float*)v, dest);
}
void nu_vec3f_one(nu_vec3f_t v)
{
    glm_vec3_one(v);
}
void nu_vec3f_zero(nu_vec3f_t v)
{
    glm_vec3_zero(v);
}
void nu_vec3f_add(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest)
{
    glm_vec3_add((float*)a, (float*)b, dest);
}
void nu_vec3f_adds(const nu_vec3f_t v, float s, nu_vec3f_t dest)
{
    glm_vec3_adds((float*)v, s, dest);
}
void nu_vec3f_sub(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest)
{
    glm_vec3_sub((float*)a, (float*)b, dest);
}
void nu_vec3f_subs(const nu_vec3f_t v, float s, nu_vec3f_t dest)
{
    glm_vec3_subs((float*)v, s, dest);
}
void nu_vec3f_mul(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest)
{
    glm_vec3_mul((float*)a, (float*)b, dest);
}
void nu_vec3f_muls(const nu_vec3f_t v, float s, nu_vec3f_t dest)
{
    glm_vec3_scale((float*)v, s, dest);
}
void nu_vec3f_div(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest)
{
    glm_vec3_div((float*)a, (float*)b, dest);
}
void nu_vec3f_divs(const nu_vec3f_t v, float s, nu_vec3f_t dest)
{
    glm_vec3_divs((float*)v, s, dest);
}
float nu_vec3f_dot(const nu_vec3f_t a, const nu_vec3f_t b)
{
    return glm_vec3_dot((float*)a, (float*)b);
}
float nu_vec3f_length(const nu_vec3f_t v)
{
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
void nu_vec3f_normalize(nu_vec3f_t v)
{
    glm_vec3_normalize(v);
}
void nu_vec3f_cross(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest)
{
    glm_vec3_cross((float*)a, (float*)b, dest);
}
void nu_vec3f_lerp(const nu_vec3f_t from, const nu_vec3f_t to, float t, nu_vec3f_t dest)
{
    glm_vec3_lerp((float*)from, (float*)to, t, dest);
}
void nu_vec3f_abs(nu_vec3f_t v)
{
    glm_vec3_abs(v, v);
}
void nu_vec3f_maxv(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest)
{
    glm_vec3_maxv((float*)a, (float*)b, dest);
}
void nu_vec3f_minv(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest)
{
    glm_vec3_minv((float*)a, (float*)b, dest);
}