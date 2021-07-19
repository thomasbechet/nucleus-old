#include <nucleus/core/utils/transform.h>

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
    
}
void nu_transform_get_forward_vector(const nu_transform_t *transform, nu_vec3f_t vec)
{

}
void nu_transform_get_backward_vector(const nu_transform_t *transform, nu_vec3f_t vec)
{

}
void nu_transform_get_left_vector(const nu_transform_t *transform, nu_vec3f_t vec)
{

}
void nu_transform_get_right_vector(const nu_transform_t *transform, nu_vec3f_t vec)
{

}
void nu_transform_get_up_vector(const nu_transform_t *transform, nu_vec3f_t vec)
{

}
void nu_transform_get_down_vector(const nu_transform_t *transform, nu_vec3f_t vec)
{

}