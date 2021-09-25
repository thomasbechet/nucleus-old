#include <nucleus/core/utility/math/transform.h>

#include <nucleus/core/utility/math/mat4.h>
#include <nucleus/core/utility/math/vec3.h>
#include <nucleus/core/utility/math/quat.h>

void nu_transform_identity(nu_transform_t *transform)
{
    nu_vec3f_zero(transform->translation);
    nu_vec3f_one(transform->scale);
    nu_quatf_identity(transform->rotation);
}
void nu_transform_build_matrix(const nu_vec3f_t translation, const nu_quatf_t rotation, const nu_vec3f_t scale, nu_mat4f_t matrix)
{
    nu_mat4f_identity(matrix);
    nu_mat4f_scale(matrix, scale);
    nu_mat4f_rotate(matrix, rotation);
    nu_mat4f_translate(matrix, translation);
}
void nu_transform_build_direction(const nu_quatf_t rotation, const nu_vec3f_t direction, nu_vec3f_t dest)
{
    nu_vec3f_copy(direction, dest);
    nu_quatf_mulv(rotation, dest, dest);
    nu_vec3f_normalize(dest);
}
void nu_transform_get_matrix(const nu_transform_t *transform, nu_mat4f_t matrix)
{
    nu_transform_build_matrix(transform->translation, transform->rotation, transform->scale, matrix);
}
void nu_transform_get_forward(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_transform_build_direction(transform->rotation, NU_VEC3F_FORWARD, vec);
}
void nu_transform_get_backward(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_transform_build_direction(transform->rotation, NU_VEC3F_BACKWARD, vec);
}
void nu_transform_get_left(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_transform_build_direction(transform->rotation, NU_VEC3F_LEFT, vec);
}
void nu_transform_get_right(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_transform_build_direction(transform->rotation, NU_VEC3F_RIGHT, vec);
}
void nu_transform_get_up(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_transform_build_direction(transform->rotation, NU_VEC3F_UP, vec);
}
void nu_transform_get_down(const nu_transform_t *transform, nu_vec3f_t vec)
{
    nu_transform_build_direction(transform->rotation, NU_VEC3F_DOWN, vec);
}