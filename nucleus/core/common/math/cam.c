#include "cam.h"

#include <cglm/cglm.h>

void nu_perspective(float fovy, float aspect, float near, float far, nu_mat4_t dest)
{
    glm_perspective(fovy, aspect, near, far, (vec4*)dest);
}
void nu_lookat(const nu_vec3_t eye, const nu_vec3_t center, const nu_vec3_t up, nu_mat4_t dest)
{
    glm_lookat((float*)eye, (float*)center, (float*)up, (vec4*)dest);
}