#ifndef NUGLFW_SURFACE_H
#define NUGLFW_SURFACE_H

#include <nucleus/nucleus.h>

nu_result_t nuglfw_surface_create(void);
nu_result_t nuglfw_surface_destroy(void);
nu_result_t nuglfw_surface_draw(
    uint32_t width,
    uint32_t height,
    void *pixels
);

#endif