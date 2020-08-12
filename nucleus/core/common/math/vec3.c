#include "vec3.h"

#include <cglm/cglm.h>

void nu_vec3_copy(const nu_vec3_t v, nu_vec3_t dest)
{
    glm_vec3_copy((float*)v, dest);
}
void nu_vec3_one(nu_vec3_t v)
{
    glm_vec3_one(v);
}
void nu_vec3_zero(nu_vec3_t v)
{
    glm_vec3_zero(v);
}
void nu_vec3_add(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest)
{
    glm_vec3_add((float*)a, (float*)b, dest);
}
void nu_vec3_adds(const nu_vec3_t v, float s, nu_vec3_t dest)
{
    glm_vec3_adds((float*)v, s, dest);
}
void nu_vec3_sub(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest)
{
    glm_vec3_sub((float*)a, (float*)b, dest);
}
void nu_vec3_subs(const nu_vec3_t v, float s, nu_vec3_t dest)
{
    glm_vec3_subs((float*)v, s, dest);
}
void nu_vec3_mul(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest)
{
    glm_vec3_mul((float*)a, (float*)b, dest);
}
void nu_vec3_muls(const nu_vec3_t v, float s, nu_vec3_t dest)
{
    glm_vec3_scale((float*)v, s, dest);
}
void nu_vec3_div(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest)
{
    glm_vec3_div((float*)a, (float*)b, dest);
}
void nu_vec3_divs(const nu_vec3_t v, float s, nu_vec3_t dest)
{
    glm_vec3_divs((float*)v, s, dest);
}
float nu_vec3_dot(const nu_vec3_t a, const nu_vec3_t b)
{
    return glm_vec3_dot((float*)a, (float*)b);
}
void nu_vec3_normalize(nu_vec3_t v)
{
    glm_vec3_normalize(v);
}
void nu_vec3_cross(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest)
{
    glm_vec3_cross((float*)a, (float*)b, dest);
}
void nu_vec3_lerp(const nu_vec3_t from, const nu_vec3_t to, float t, nu_vec3_t dest)
{
    glm_vec3_lerp((float*)from, (float*)to, t, dest);
}