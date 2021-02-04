#ifndef NU_VEC2_H
#define NU_VEC2_H

#include "types.h"

NU_API void nu_vec2f_copy(const nu_vec2f_t v, nu_vec2f_t dest);
NU_API void nu_vec2f_one(nu_vec2f_t v);
NU_API void nu_vec2f_zero(nu_vec2f_t v);
NU_API void nu_vec2f_add(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest);
NU_API void nu_vec2f_adds(const nu_vec2f_t v, float s, nu_vec2f_t dest);
NU_API void nu_vec2f_sub(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest);
NU_API void nu_vec2f_subs(const nu_vec2f_t v, float s, nu_vec2f_t dest);
NU_API void nu_vec2f_mul(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest);
NU_API void nu_vec2f_muls(const nu_vec2f_t v, float s, nu_vec2f_t dest);
NU_API void nu_vec2f_div(const nu_vec2f_t a, const nu_vec2f_t b, nu_vec2f_t dest);
NU_API void nu_vec2f_divs(const nu_vec2f_t v, float s, nu_vec2f_t dest);
NU_API float nu_vec2f_length(const nu_vec2f_t v);
NU_API void nu_vec2f_normalize(nu_vec2f_t v);
NU_API void nu_vec2f_lerp(const nu_vec2f_t from, const nu_vec2f_t to, float t, nu_vec2f_t dest);

NU_API void nu_vec2i_copy(const nu_vec2i_t v, nu_vec2i_t dest);
NU_API void nu_vec2i_one(nu_vec2i_t v);
NU_API void nu_vec2i_zero(nu_vec2i_t v);
NU_API void nu_vec2i_add(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest);
NU_API void nu_vec2i_adds(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest);
NU_API void nu_vec2i_sub(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest);
NU_API void nu_vec2i_subs(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest);
NU_API void nu_vec2i_mul(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest);
NU_API void nu_vec2i_muls(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest);
NU_API void nu_vec2i_div(const nu_vec2i_t a, const nu_vec2i_t b, nu_vec2i_t dest);
NU_API void nu_vec2i_divs(const nu_vec2i_t v, int32_t s, nu_vec2i_t dest);

NU_API void nu_vec2u_copy(const nu_vec2u_t v, nu_vec2u_t dest);
NU_API void nu_vec2u_one(nu_vec2u_t v);
NU_API void nu_vec2u_zero(nu_vec2u_t v);
NU_API void nu_vec2u_add(const nu_vec2u_t a, const nu_vec2u_t b, nu_vec2u_t dest);
NU_API void nu_vec2u_adds(const nu_vec2u_t v, uint32_t s, nu_vec2u_t dest);
NU_API void nu_vec2u_sub(const nu_vec2u_t a, const nu_vec2u_t b, nu_vec2u_t dest);
NU_API void nu_vec2u_subs(const nu_vec2u_t v, uint32_t s, nu_vec2u_t dest);
NU_API void nu_vec2u_mul(const nu_vec2u_t a, const nu_vec2u_t b, nu_vec2u_t dest);
NU_API void nu_vec2u_muls(const nu_vec2u_t v, uint32_t s, nu_vec2u_t dest);
NU_API void nu_vec2u_div(const nu_vec2u_t a, const nu_vec2u_t b, nu_vec2u_t dest);
NU_API void nu_vec2u_divs(const nu_vec2u_t v, uint32_t s, nu_vec2u_t dest);

#endif