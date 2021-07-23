#ifndef NU_VEC3_H
#define NU_VEC3_H

#include <nucleus/core/common/math/types.h>

#define NU_VEC3F_FORWARD  (nu_vec3f_t){ 0,  0, -1}
#define NU_VEC3F_BACKWARD (nu_vec3f_t){ 0,  0,  1}
#define NU_VEC3F_LEFT     (nu_vec3f_t){-1,  0,  0}
#define NU_VEC3F_RIGHT    (nu_vec3f_t){ 1,  0,  0}
#define NU_VEC3F_UP       (nu_vec3f_t){ 0,  1,  0}
#define NU_VEC3F_DOWN     (nu_vec3f_t){ 0, -1,  0}

NU_API void nu_vec3f_copy(const nu_vec3f_t v, nu_vec3f_t dest);
NU_API void nu_vec3f_one(nu_vec3f_t v);
NU_API void nu_vec3f_zero(nu_vec3f_t v);
NU_API void nu_vec3f_add(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest);
NU_API void nu_vec3f_adds(const nu_vec3f_t v, float s, nu_vec3f_t dest);
NU_API void nu_vec3f_sub(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest);
NU_API void nu_vec3f_subs(const nu_vec3f_t v, float s, nu_vec3f_t dest);
NU_API void nu_vec3f_mul(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest);
NU_API void nu_vec3f_muls(const nu_vec3f_t v, float s, nu_vec3f_t dest);
NU_API void nu_vec3f_div(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest);
NU_API void nu_vec3f_divs(const nu_vec3f_t v, float s, nu_vec3f_t dest);
NU_API float nu_vec3f_dot(const nu_vec3f_t a, const nu_vec3f_t b);
NU_API float nu_vec3f_length(const nu_vec3f_t v);
NU_API void nu_vec3f_normalize(nu_vec3f_t v);
NU_API void nu_vec3f_cross(const nu_vec3f_t a, const nu_vec3f_t b, nu_vec3f_t dest);
NU_API void nu_vec3f_lerp(const nu_vec3f_t from, const nu_vec3f_t to, float t, nu_vec3f_t dest);

#endif