#ifndef NUUTILS_LOADER_H
#define NUUTILS_LOADER_H

#include <nucleus/module/utils/module/interface.h>

/* loader interface */
nu_result_t nuutils_loader_load_mesh_from_obj(const char *filename, nu_renderer_mesh_t *handle);
nu_result_t nuutils_loader_load_texture(const char *filename, nu_renderer_texture_t *handle);

#endif