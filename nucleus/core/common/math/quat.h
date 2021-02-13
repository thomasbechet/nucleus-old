#ifndef NU_QUAT_H
#define NU_QUAT_H

#include <nucleus/core/common/math/types.h>
#include <nucleus/core/common/math/mat4.h>

NU_API void nu_quatf_copy(const nu_quatf_t q, nu_quatf_t dest);
NU_API void nu_quatf_identity(nu_quatf_t q);
NU_API void nu_quatf_from_axis(float angle, const nu_vec3f_t axis, nu_quatf_t dest);
NU_API void nu_quatf_to_mat4(const nu_quatf_t q, nu_mat4f_t dest);
NU_API void nu_quatf_to_mat3(const nu_quatf_t q, nu_mat3f_t dest);
NU_API void nu_quatf_mul(const nu_quatf_t a, const nu_quatf_t b, nu_quatf_t dest);
NU_API void nu_quatf_mulv(const nu_quatf_t q, const nu_vec3f_t v, nu_quatf_t dest); 
NU_API void nu_quatf_lerp(const nu_quatf_t a, const nu_quatf_t b, float t, nu_quatf_t dest);
NU_API void nu_quatf_slerp(const nu_quatf_t a, const nu_quatf_t b, float t, nu_quatf_t dest);

#endif