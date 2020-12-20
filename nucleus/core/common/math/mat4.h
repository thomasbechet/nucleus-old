#ifndef NU_MAT4_H
#define NU_MAT4_H

#include "types.h"
#include "mat3.h"

NU_API void nu_mat4_copy(const nu_mat4_t m, nu_mat4_t dest);
NU_API void nu_mat4_mul(const nu_mat4_t m0, const nu_mat4_t m1, nu_mat4_t dest);
NU_API void nu_mat4_mulv(const nu_mat4_t m, const nu_vec4_t v, nu_vec4_t dest);
NU_API void nu_mat4_mulv3(const nu_mat4_t m, const nu_vec3_t v, float w, nu_vec3_t dest);
NU_API void nu_mat4_identity(nu_mat4_t m);
NU_API void nu_translate(nu_mat4_t m, const nu_vec3_t translate);
NU_API void nu_rotate(nu_mat4_t m, float angle, const nu_vec3_t axis);
NU_API void nu_scale(nu_mat4_t m, const nu_vec3_t scale);
NU_API void nu_mat4_pick_mat3(const nu_mat4_t m, nu_mat3_t dest);
NU_API void nu_mat4_inv(const nu_mat4_t m, nu_mat4_t dest);

#endif