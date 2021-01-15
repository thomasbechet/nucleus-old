#ifndef NUUTILS_LOADER_INTERFACE_H
#define NUUTILS_LOADER_INTERFACE_H

#include <nucleus/nucleus.h>

/* loader plugin */
#define NUUTILS_LOADER_PLUGIN_NAME "nuutils_loader_plugin"
#define NUUTILS_LOADER_INTERFACE_NAME "nuutils_loader_interface"
#define NUUTILS_LOADER_INTERFACE_LOADER_NAME "nuutils_loader_get_interface"

typedef struct {
    nu_result_t (*load_mesh_from_obj)(nu_renderer_mesh_handle_t*, const char*);
    nu_result_t (*load_texture)(nu_renderer_texture_handle_t*, const char*);
} nuutils_loader_interface_t;

typedef nu_result_t (*nuutils_loader_interface_loader_pfn_t)(nuutils_loader_interface_t*);

#endif