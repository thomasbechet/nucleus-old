#pragma once

#include "../module/interface.h"

/* loader interface */
nu_result_t load_mesh_from_obj(nu_renderer_mesh_handle_t *handle, const char *filename);
nu_result_t load_texture(nu_renderer_texture_handle_t *handle, const char *filename);