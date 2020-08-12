#include "vec4.h"

#include <cglm/cglm.h>

void nu_vec4(const nu_vec3_t v, float w, nu_vec4_t dest)
{
    glm_vec4((float*)v, w, dest);
}
void nu_vec4_copy(const nu_vec4_t v, nu_vec4_t dest)
{
    glm_vec4_copy((float*)v, dest);
}
void nu_vec4_one(nu_vec4_t v)
{
    glm_vec4_one(v);
}
void nu_vec4_zero(nu_vec4_t v)
{
    glm_vec4_zero(v);
}
void nu_vec4_add(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest)
{
    glm_vec4_add((float*)a, (float*)b, dest);
}
void nu_vec4_adds(const nu_vec4_t v, float s, nu_vec4_t dest)
{
    glm_vec4_adds((float*)v, s, dest);
}
void nu_vec4_sub(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest)
{
    glm_vec4_sub((float*)a, (float*)b, dest);
}
void nu_vec4_subs(const nu_vec4_t v, float s, nu_vec4_t dest)
{
    glm_vec4_subs((float*)v, s, dest);
}
void nu_vec4_mul(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest)
{
    glm_vec4_mul((float*)a, (float*)b, dest);
}
void nu_vec4_muls(const nu_vec4_t v, float s, nu_vec4_t dest)
{
    glm_vec4_scale((float*)v, s, dest);
}
void nu_vec4_div(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest)
{
    glm_vec4_div((float*)a, (float*)b, dest);
}
void nu_vec4_divs(const nu_vec4_t v, float s, nu_vec4_t dest)
{
    glm_vec4_divs((float*)v, s, dest);
}
float nu_vec4_dot(const nu_vec4_t a, const nu_vec4_t b)
{
    return glm_vec4_dot((float*)a, (float*)b);
}
void nu_vec4_normalize(nu_vec4_t v)
{
    glm_vec4_normalize(v);
}
void nu_vec4_lerp(const nu_vec4_t from, const nu_vec4_t to, float t, nu_vec4_t dest)
{
    glm_vec4_lerp((float*)from, (float*)to, t, dest);
}