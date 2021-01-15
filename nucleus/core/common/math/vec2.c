#include "vec2.h"

#include <cglm/cglm.h>

void nu_vec2f_copy(const nu_vec2f_t v, nu_vec2f_t dest)
{
    glm_vec2_copy((float*)v, dest);
}
void nu_vec2f_one(nu_vec2f_t v)
{
    glm_vec2_one(v);
}
void nu_vec2f_zero(nu_vec2f_t v)
{
    glm_vec2_zero(v);
}
void nu_vec2f_add(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest)
{
    glm_vec2_add((float*)a, (float*)b, dest);
}
void nu_vec2f_adds(const nu_vec2f_t v, float s, nu_vec2f_t dest)
{
    glm_vec2_adds((float*)v, s, dest);
}
void nu_vec2f_sub(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest)
{
    glm_vec2_sub((float*)a, (float*)b, dest);
}
void nu_vec2f_subs(const nu_vec2f_t v, float s, nu_vec2f_t dest)
{
    glm_vec2_subs((float*)v, s, dest);
}
void nu_vec2f_mul(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest)
{
    glm_vec2_mul((float*)a, (float*)b, dest);
}
void nu_vec2f_muls(const nu_vec2f_t v, float s, nu_vec2f_t dest)
{
    glm_vec2_scale((float*)v, s, dest);
}
void nu_vec2f_div(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest)
{
    glm_vec2_div((float*)a, (float*)b, dest);
}
void nu_vec2f_divs(const nu_vec2f_t v, float s, nu_vec2f_t dest)
{
    glm_vec2_divs((float*)v, s, dest);
}
float nu_vec2f_length(const nu_vec2f_t v)
{
    return sqrtf(v[0] * v[0] + v[1] * v[1]);
}
void nu_vec2f_normalize(nu_vec2f_t v)
{
    glm_vec2_normalize(v);
}
void nu_vec2f_lerp(const nu_vec2f_t from, const nu_vec2f_t to, float t, nu_vec2f_t dest)
{
    glm_vec2_lerp((float*)from, (float*)to, t, dest);
}

void nu_vec2i_copy(const nu_vec2i_t v, nu_vec2i_t dest)
{
    dest[0] = v[0];
    dest[1] = v[1];
}
void nu_vec2i_one(nu_vec2i_t v)
{
    v[0] = 1;
    v[1] = 1;
}
void nu_vec2i_zero(nu_vec2i_t v)
{
    v[0] = 0;
    v[1] = 0;
}
void nu_vec2i_add(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest)
{
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
}
void nu_vec2i_adds(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest)
{
    dest[0] = v[0] + s;
    dest[1] = v[1] + s;
}
void nu_vec2i_sub(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest)
{
    dest[0] = a[0] - b[0];
    dest[1] = a[1] - b[1];
}
void nu_vec2i_subs(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest)
{
    dest[0] = v[0] - s;
    dest[1] = v[1] - s;
}
void nu_vec2i_mul(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest)
{
    dest[0] = a[0] * b[0];
    dest[1] = a[1] * b[1];
}
void nu_vec2i_muls(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest)
{
    dest[0] = v[0] * s;
    dest[1] = v[1] * s;
}
void nu_vec2i_div(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest)
{
    dest[0] = a[0] / b[0];
    dest[1] = a[1] / b[1];
}
void nu_vec2i_divs(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest)
{
    dest[0] = v[0] / s;
    dest[1] = v[1] / s;
}