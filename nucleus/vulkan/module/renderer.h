#ifndef NUVK_MODULE_RENDERER_H
#define NUVK_MODULE_RENDERER_H

#include "../../core/nucleus_core.h"

/* module */
NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);

/* loaders */
NU_API nu_result_t nu_renderer_interface_loader(nu_renderer_interface_t *interface);

/* renderer interface */
NU_API nu_result_t nu_renderer_initialize(void);
NU_API nu_result_t nu_renderer_terminate(void);
NU_API nu_result_t nu_renderer_render(void);

#endif