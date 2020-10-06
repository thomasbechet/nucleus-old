#ifndef NU_MAT3_H
#define NU_MAT3_H

#include "types.h"

NU_API void nu_mat3_copy(const nu_mat3_t m, nu_mat3_t dest);
NU_API void nu_mat3_mul(const nu_mat3_t m0, const nu_mat3_t m1, nu_mat3_t dest);
NU_API void nu_mat3_mulv(const nu_mat3_t m, const nu_vec3_t v, nu_vec3_t dest);
NU_API void nu_mat3_identity(nu_mat3_t m);
NU_API void nu_mat3_inverse(nu_mat3_t m);
NU_API void nu_mat3_transpose(nu_mat3_t m);

#endif