#ifndef NU_MAT4_H
#define NU_MAT4_H

#include <nucleus/core/common/math/types.h>
#include <nucleus/core/common/math/mat3.h>

NU_API void nu_mat4f_copy(const nu_mat4f_t m, nu_mat4f_t dest);
NU_API void nu_mat4f_mul(const nu_mat4f_t m0, const nu_mat4f_t m1, nu_mat4f_t dest);
NU_API void nu_mat4f_mulv(const nu_mat4f_t m, const nu_vec4f_t v, nu_vec4f_t dest);
NU_API void nu_mat4f_mulv3(const nu_mat4f_t m, const nu_vec3f_t v, float w, nu_vec3f_t dest);
NU_API void nu_mat4f_identity(nu_mat4f_t m);
NU_API void nu_mat4f_translate(nu_mat4f_t m, const nu_vec3f_t translate);
NU_API void nu_mat4f_rotate(nu_mat4f_t m, const nu_quatf_t rotation);
NU_API void nu_mat4f_rotate_axis(nu_mat4f_t m, float angle, const nu_vec3f_t axis);
NU_API void nu_mat4f_scale(nu_mat4f_t m, const nu_vec3f_t scale);
NU_API void nu_mat4f_pick_mat3(const nu_mat4f_t m, nu_mat3f_t dest);
NU_API void nu_mat4f_inv(const nu_mat4f_t m, nu_mat4f_t dest);
NU_API void nu_mat4f_perspective(float fovy, float aspect, float near, float far, nu_mat4f_t dest);
NU_API void nu_mat4f_lookat(const nu_vec3f_t eye, const nu_vec3f_t center, const nu_vec3f_t up, nu_mat4f_t dest);

#endif