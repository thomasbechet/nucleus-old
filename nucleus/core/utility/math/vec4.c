#include <nucleus/core/utility/math/vec4.h>

#include <cglm/cglm.h>

void nu_vec4f_from_v3(const nu_vec3f_t v, float w, nu_vec4f_t dest)
{
    glm_vec4((float*)v, w, dest);
}
void nu_vec4f_copy(const nu_vec4f_t v, nu_vec4f_t dest)
{
    glm_vec4_copy((float*)v, dest);
}
void nu_vec4f_one(nu_vec4f_t v)
{
    glm_vec4_one(v);
}
void nu_vec4f_zero(nu_vec4f_t v)
{
    glm_vec4_zero(v);
}
void nu_vec4f_add(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest)
{
    glm_vec4_add((float*)a, (float*)b, dest);
}
void nu_vec4f_adds(const nu_vec4f_t v, float s, nu_vec4f_t dest)
{
    glm_vec4_adds((float*)v, s, dest);
}
void nu_vec4f_sub(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest)
{
    glm_vec4_sub((float*)a, (float*)b, dest);
}
void nu_vec4f_subs(const nu_vec4f_t v, float s, nu_vec4f_t dest)
{
    glm_vec4_subs((float*)v, s, dest);
}
void nu_vec4f_mul(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest)
{
    glm_vec4_mul((float*)a, (float*)b, dest);
}
void nu_vec4f_muls(const nu_vec4f_t v, float s, nu_vec4f_t dest)
{
    glm_vec4_scale((float*)v, s, dest);
}
void nu_vec4f_div(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest)
{
    glm_vec4_div((float*)a, (float*)b, dest);
}
void nu_vec4f_divs(const nu_vec4f_t v, float s, nu_vec4f_t dest)
{
    glm_vec4_divs((float*)v, s, dest);
}
float nu_vec4f_dot(const nu_vec4f_t a, const nu_vec4f_t b)
{
    return glm_vec4_dot((float*)a, (float*)b);
}
void nu_vec4f_normalize(nu_vec4f_t v)
{
    glm_vec4_normalize(v);
}
void nu_vec4f_lerp(const nu_vec4f_t from, const nu_vec4f_t to, float t, nu_vec4f_t dest)
{
    glm_vec4_lerp((float*)from, (float*)to, t, dest);
}

void nu_vec4i_copy(const nu_vec4i_t v, nu_vec4i_t dest)
{
    dest[0] = v[0];
    dest[1] = v[1];
    dest[2] = v[2];
    dest[3] = v[3];
}
void nu_vec4i_one(nu_vec4i_t v)
{
    v[0] = 1;
    v[1] = 1;
    v[2] = 1;
    v[3] = 1;
}
void nu_vec4i_zero(nu_vec4i_t v)
{
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    v[3] = 0;
}
void nu_vec4i_add(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest)
{
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
    dest[3] = a[3] + b[3];
}
void nu_vec4i_adds(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest)
{
    dest[0] = v[0] + s;
    dest[1] = v[1] + s;
    dest[2] = v[2] + s;
    dest[3] = v[3] + s;
}
void nu_vec4i_sub(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest)
{
    dest[0] = a[0] - b[0];
    dest[1] = a[1] - b[1];
    dest[2] = a[2] - b[2];
    dest[3] = a[3] - b[3];
}
void nu_vec4i_subs(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest)
{
    dest[0] = v[0] - s;
    dest[1] = v[1] - s;
    dest[2] = v[2] - s;
    dest[3] = v[3] - s;
}
void nu_vec4i_mul(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest)
{
    dest[0] = a[0] * b[0];
    dest[1] = a[1] * b[1];
    dest[2] = a[2] * b[2];
    dest[3] = a[3] * b[3];
}
void nu_vec4i_muls(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest)
{
    dest[0] = v[0] * s;
    dest[1] = v[1] * s;
    dest[2] = v[2] * s;
    dest[3] = v[3] * s;
}
void nu_vec4i_div(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest)
{
    dest[0] = a[0] / b[0];
    dest[1] = a[1] / b[1];
    dest[2] = a[2] / b[2];
    dest[3] = a[3] / b[3];
}
void nu_vec4i_divs(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest)
{
    dest[0] = v[0] / s;
    dest[1] = v[1] / s;
    dest[2] = v[2] / s;
    dest[3] = v[3] / s;
}