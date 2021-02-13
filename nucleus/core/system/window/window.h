#ifndef NU_WINDOW_H
#define NU_WINDOW_H

#include <nucleus/core/system/window/interface.h>

nu_result_t nu_window_initialize(void);
nu_result_t nu_window_terminate(void);
nu_result_t nu_window_start(void);
nu_result_t nu_window_stop(void);
nu_result_t nu_window_update(void);

#endif