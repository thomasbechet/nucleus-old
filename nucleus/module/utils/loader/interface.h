#ifndef NUUTILS_LOADER_INTERFACE_H
#define NUUTILS_LOADER_INTERFACE_H

#include <nucleus/nucleus.h>

#define NUUTILS_LOADER_INTERFACE_NAME "nuutils_loader_interface"

typedef struct {
    nu_result_t (*load_mesh_from_obj)(const char*, nu_renderer_mesh_t*);
    nu_result_t (*load_texture)(const char*, nu_renderer_texture_t*);
} nuutils_loader_interface_t;

#endif