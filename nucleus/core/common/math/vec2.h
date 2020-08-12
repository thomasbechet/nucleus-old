#ifndef NU_VEC2_H
#define NU_VEC2_H

#include "types.h"

NU_API void nu_vec2_copy(const nu_vec2_t v, nu_vec2_t dest);
NU_API void nu_vec2_one(nu_vec2_t v);
NU_API void nu_vec2_zero(nu_vec2_t v);
NU_API void nu_vec2_add(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest);
NU_API void nu_vec2_adds(const nu_vec2_t v, float s, nu_vec2_t dest);
NU_API void nu_vec2_sub(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest);
NU_API void nu_vec2_subs(const nu_vec2_t v, float s, nu_vec2_t dest);
NU_API void nu_vec2_mul(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest);
NU_API void nu_vec2_muls(const nu_vec2_t v, float s, nu_vec2_t dest);
NU_API void nu_vec2_div(const nu_vec2_t a, const nu_vec2_t b, nu_vec2_t dest);
NU_API void nu_vec2_divs(const nu_vec2_t v, float s, nu_vec2_t dest);
NU_API void nu_vec2_lerp(const nu_vec4_t from, const nu_vec4_t to, float t, nu_vec4_t dest);

#endif