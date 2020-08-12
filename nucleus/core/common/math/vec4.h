#ifndef NU_VEC4_H
#define NU_VEC4_H

#include "types.h"

NU_API void nu_vec4(const nu_vec3_t v, float w, nu_vec4_t dest);
NU_API void nu_vec4_copy(const nu_vec4_t v, nu_vec4_t dest);
NU_API void nu_vec4_one(nu_vec4_t v);
NU_API void nu_vec4_zero(nu_vec4_t v);
NU_API void nu_vec4_add(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest);
NU_API void nu_vec4_adds(const nu_vec4_t v, float s, nu_vec4_t dest);
NU_API void nu_vec4_sub(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest);
NU_API void nu_vec4_subs(const nu_vec4_t v, float s, nu_vec4_t dest);
NU_API void nu_vec4_mul(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest);
NU_API void nu_vec4_muls(const nu_vec4_t v, float s, nu_vec4_t dest);
NU_API void nu_vec4_div(const nu_vec4_t a, const nu_vec4_t b, nu_vec4_t dest);
NU_API void nu_vec4_divs(const nu_vec4_t v, float s, nu_vec4_t dest);
NU_API float nu_vec4_dot(const nu_vec4_t a, const nu_vec4_t b);
NU_API void nu_vec4_normalize(nu_vec4_t v);
NU_API void nu_vec4_lerp(const nu_vec4_t from, const nu_vec4_t to, float t, nu_vec4_t dest);

#endif