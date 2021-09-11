#ifndef NU_VEC4_H
#define NU_VEC4_H

#include <nucleus/core/utility/math/types.h>

NU_API void nu_vec4f_from_v3(const nu_vec3f_t v, float w, nu_vec4f_t dest);
NU_API void nu_vec4f_copy(const nu_vec4f_t v, nu_vec4f_t dest);
NU_API void nu_vec4f_one(nu_vec4f_t v);
NU_API void nu_vec4f_zero(nu_vec4f_t v);
NU_API void nu_vec4f_add(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest);
NU_API void nu_vec4f_adds(const nu_vec4f_t v, float s, nu_vec4f_t dest);
NU_API void nu_vec4f_sub(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest);
NU_API void nu_vec4f_subs(const nu_vec4f_t v, float s, nu_vec4f_t dest);
NU_API void nu_vec4f_mul(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest);
NU_API void nu_vec4f_muls(const nu_vec4f_t v, float s, nu_vec4f_t dest);
NU_API void nu_vec4f_div(const nu_vec4f_t a, const nu_vec4f_t b, nu_vec4f_t dest);
NU_API void nu_vec4f_divs(const nu_vec4f_t v, float s, nu_vec4f_t dest);
NU_API float nu_vec4f_dot(const nu_vec4f_t a, const nu_vec4f_t b);
NU_API void nu_vec4f_normalize(nu_vec4f_t v);
NU_API void nu_vec4f_lerp(const nu_vec4f_t from, const nu_vec4f_t to, float t, nu_vec4f_t dest);

NU_API void nu_vec4i_copy(const nu_vec4i_t v, nu_vec4i_t dest);
NU_API void nu_vec4i_one(nu_vec4i_t v);
NU_API void nu_vec4i_zero(nu_vec4i_t v);
NU_API void nu_vec4i_add(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest);
NU_API void nu_vec4i_adds(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest);
NU_API void nu_vec4i_sub(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest);
NU_API void nu_vec4i_subs(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest);
NU_API void nu_vec4i_mul(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest);
NU_API void nu_vec4i_muls(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest);
NU_API void nu_vec4i_div(const nu_vec4i_t a, const nu_vec4i_t b, nu_vec4i_t dest);
NU_API void nu_vec4i_divs(const nu_vec4i_t v, int32_t s, nu_vec4i_t dest);

#endif