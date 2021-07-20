#include <nucleus/core/common/math/transform.h>

#include <nucleus/core/common/math/mat4.h>
#include <nucleus/core/common/math/vec3.h>
#include <nucleus/core/common/math/quat.h>

void nu_transform_identity(nu_transform_t *transform)
{
    nu_vec3f_zero(transform->translation);
    nu_vec3f_one(transform->scale);
    nu_quatf_identity(transform->rotation);
}
void nu_transform_rotate(nu_transform_t *transform, float angle, const nu_vec3f_t axis)
{
    nu_quatf_t rotation;
    nu_quatf_from_axis(angle, axis, rotation);
    nu_quatf_mul(transform->rotation, rotation, transform->rotation);
}
void nu_transform_translate(nu_transform_t *transform, const nu_vec3f_t translation)
{
    nu_vec3f_add(transform->translation, translation, transform->translation);
}
void nu_transform_get_matrix(const nu_transform_t *transform, nu_mat4f_t matrix)
{
    nu_mat4f_identity(matrix);
    nu_mat4f_scale(matrix, transform->scale);
    nu_mat4f_rotate(matrix, transform->rotation);
    nu_mat4f_translate(matrix, transform->translation);
}
void nu_transform_get_forward(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_vec3f_copy(NU_VEC3F_FORWARD, vec);
    nu_quatf_mulv(transform->rotation, vec, vec);
    nu_vec3f_normalize(vec);
}
void nu_transform_get_backward(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_vec3f_copy(NU_VEC3F_BACKWARD, vec);
    nu_quatf_mulv(transform->rotation, vec, vec);
    nu_vec3f_normalize(vec);
}
void nu_transform_get_left(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_vec3f_copy(NU_VEC3F_LEFT, vec);
    nu_quatf_mulv(transform->rotation, vec, vec);
    nu_vec3f_normalize(vec);
}
void nu_transform_get_right(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_vec3f_copy(NU_VEC3F_RIGHT, vec);
    nu_quatf_mulv(transform->rotation, vec, vec);
    nu_vec3f_normalize(vec);
}
void nu_transform_get_up(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_vec3f_copy(NU_VEC3F_UP, vec);
    nu_quatf_mulv(transform->rotation, vec, vec);
    nu_vec3f_normalize(vec);
}
void nu_transform_get_down(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_vec3f_copy(NU_VEC3F_DOWN, vec);
    nu_quatf_mulv(transform->rotation, vec, vec);
    nu_vec3f_normalize(vec);
}