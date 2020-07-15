#ifndef NUSR_RENDERER_H
#define NUSR_RENDERER_H

#include "interface.h"

/* module */
NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);

/* loaders */
NU_API nu_result_t nu_renderer_interface_loader(nu_renderer_interface_t *interface);
NU_API nu_result_t nusr_renderer_interface_loader(nusr_renderer_interface_t *interface);

#endif