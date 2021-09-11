#ifndef NU_MAT3_H
#define NU_MAT3_H

#include <nucleus/core/utility/math/types.h>

NU_API void nu_mat3f_copy(const nu_mat3f_t m, nu_mat3f_t dest);
NU_API void nu_mat3f_mul(const nu_mat3f_t m0, const nu_mat3f_t m1, nu_mat3f_t dest);
NU_API void nu_mat3f_mulv(const nu_mat3f_t m, const nu_vec3f_t v, nu_vec3f_t dest);
NU_API void nu_mat3f_identity(nu_mat3f_t m);
NU_API void nu_mat3f_inverse(nu_mat3f_t m);
NU_API void nu_mat3f_transpose(nu_mat3f_t m);

#endif