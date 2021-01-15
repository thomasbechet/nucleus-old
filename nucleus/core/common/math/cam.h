#ifndef NU_CAM_H
#define NU_CAM_H

#include "types.h"

NU_API void nu_perspective(float fovy, float aspect, float near, float far, nu_mat4f_t dest);
NU_API void nu_lookat(const nu_vec3f_t eye, const nu_vec3f_t center, const nu_vec3f_t up, nu_mat4f_t dest);

#endif