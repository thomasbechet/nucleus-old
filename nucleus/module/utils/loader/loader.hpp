#pragma once

#include "../module/interface.h"

/* plugin interface */

/* loader interface */
nu_result_t load_mesh_from_obj(nu_renderer_mesh_handle_t *handle, const char *filename);
nu_result_t load_texture(nu_renderer_texture_handle_t *handle, const char *filename);

extern "C"
{
    NU_API nu_result_t nuutils_loader_get_interface(nuutils_loader_interface_t *interface);
}