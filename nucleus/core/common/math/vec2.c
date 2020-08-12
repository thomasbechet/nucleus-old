#include "vec2.h"

#include <cglm/cglm.h>

void nu_vec2_copy(const nu_vec2_t v, nu_vec2_t dest)
{
    glm_vec2_copy((float*)v, dest);
}
void nu_vec2_one(nu_vec2_t v)
{
    glm_vec2_one(v);
}
void nu_vec2_zero(nu_vec2_t v)
{
    glm_vec2_zero(v);
}
void nu_vec2_add(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest)
{
    glm_vec2_add((float*)a, (float*)b, dest);
}
void nu_vec2_adds(const nu_vec2_t v, float s, nu_vec2_t dest)
{
    glm_vec2_adds((float*)v, s, dest);
}
void nu_vec2_sub(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest)
{
    glm_vec2_sub((float*)a, (float*)b, dest);
}
void nu_vec2_subs(const nu_vec2_t v, float s, nu_vec2_t dest)
{
    glm_vec2_subs((float*)v, s, dest);
}
void nu_vec2_mul(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest)
{
    glm_vec2_mul((float*)a, (float*)b, dest);
}
void nu_vec2_muls(const nu_vec2_t v, float s, nu_vec2_t dest)
{
    glm_vec2_scale((float*)v, s, dest);
}
void nu_vec2_div(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest)
{
    glm_vec2_div((float*)a, (float*)b, dest);
}
void nu_vec2_divs(const nu_vec2_t v, float s, nu_vec2_t dest)
{
    glm_vec2_divs((float*)v, s, dest);
}
void nu_vec2_lerp(const nu_vec2_t from, const nu_vec2_t to, float t, nu_vec2_t dest)
{
    glm_vec2_lerp((float*)from, (float*)to, t, dest);
}