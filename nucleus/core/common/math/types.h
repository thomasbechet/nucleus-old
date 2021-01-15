#ifndef NU_MATH_TYPES_H
#define NU_MATH_TYPES_H

#include "../platform.h"

#ifdef __AVX__
    #define NU_ALIGN_MAT NU_ALIGN(32)
#else
    #define NU_ALIGN_MAT NU_ALIGN(16)
#endif

typedef float nu_vec2f_t[2];
typedef int32_t nu_vec2i_t[2];
typedef float nu_vec3f_t[3];
typedef NU_ALIGN(16) float nu_vec4f_t[4];
typedef NU_ALIGN(16) int32_t nu_vec4i_t[4];
typedef NU_ALIGN(16) nu_vec2f_t nu_mat2f_t[2];
typedef nu_vec3f_t nu_mat3f_t[3];
typedef NU_ALIGN_MAT nu_vec4f_t nu_mat4f_t[4];
typedef NU_ALIGN(16) float nu_quatf_t[4];

/* constants */
#define NU_PI 3.14159265358979323846264338327950288

#endif