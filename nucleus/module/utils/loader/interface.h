#ifndef NUUTILS_LOADER_INTERFACE_H
#define NUUTILS_LOADER_INTERFACE_H

#include <nucleus/nucleus.h>

#define NUUTILS_LOADER_INTERFACE_NAME "nuutils_loader_interface"

typedef struct {
    nu_result_t (*load_mesh_from_obj)(nu_renderer_mesh_t*, const char*);
    nu_result_t (*load_texture)(nu_renderer_texture_t*, const char*);
} nuutils_loader_interface_t;

#endif