#ifndef NU_VEC3_H
#define NU_VEC3_H

#include "types.h"

NU_API void nu_vec3_copy(const nu_vec3_t v, nu_vec3_t dest);
NU_API void nu_vec3_one(nu_vec3_t v);
NU_API void nu_vec3_zero(nu_vec3_t v);
NU_API void nu_vec3_add(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest);
NU_API void nu_vec3_adds(const nu_vec3_t v, float s, nu_vec3_t dest);
NU_API void nu_vec3_sub(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest);
NU_API void nu_vec3_subs(const nu_vec3_t v, float s, nu_vec3_t dest);
NU_API void nu_vec3_mul(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest);
NU_API void nu_vec3_muls(const nu_vec3_t v, float s, nu_vec3_t dest);
NU_API void nu_vec3_div(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest);
NU_API void nu_vec3_divs(const nu_vec3_t v, float s, nu_vec3_t dest);
NU_API float nu_vec3_dot(const nu_vec3_t a, const nu_vec3_t b);
NU_API void nu_vec3_normalize(nu_vec3_t v);
NU_API void nu_vec3_cross(const nu_vec3_t a, const nu_vec3_t b, nu_vec3_t dest);
NU_API void nu_vec3_lerp(const nu_vec3_t from, const nu_vec3_t to, float t, nu_vec3_t dest);

#endif