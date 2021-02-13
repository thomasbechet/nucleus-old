#ifndef NU_RENDERER_H
#define NU_RENDERER_H

#include <nucleus/core/system/renderer/interface.h>

nu_result_t nu_renderer_initialize(void);
nu_result_t nu_renderer_terminate(void);
nu_result_t nu_renderer_start(void);
nu_result_t nu_renderer_stop(void);
nu_result_t nu_renderer_render(void);

#endif