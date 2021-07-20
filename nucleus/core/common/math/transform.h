#ifndef NU_TRANSFORM_H
#define NU_TRANSFORM_H

#include <nucleus/core/common/math/types.h>

typedef struct {
    nu_vec3f_t translation;
    nu_vec3f_t scale;
    nu_quatf_t rotation;
} nu_transform_t;

NU_API void nu_transform_identity(nu_transform_t *transform);
NU_API void nu_transform_rotate(nu_transform_t *transform, float angle, const nu_vec3f_t axis);
NU_API void nu_transform_translate(nu_transform_t *transform, const nu_vec3f_t translation);
NU_API void nu_transform_get_matrix(const nu_transform_t *transform, nu_mat4f_t matrix);
NU_API void nu_transform_get_forward(const nu_transform_t *transform, nu_vec3f_t vec);
NU_API void nu_transform_get_backward(const nu_transform_t *transform, nu_vec3f_t vec);
NU_API void nu_transform_get_left(const nu_transform_t *transform, nu_vec3f_t vec);
NU_API void nu_transform_get_right(const nu_transform_t *transform, nu_vec3f_t vec);
NU_API void nu_transform_get_up(const nu_transform_t *transform, nu_vec3f_t vec);
NU_API void nu_transform_get_down(const nu_transform_t *transform, nu_vec3f_t vec);

#endif