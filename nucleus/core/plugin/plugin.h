#ifndef NU_PLUGIN_H
#define NU_PLUGIN_H

#include "interface.h"

nu_result_t nu_plugin_initialize(void);
nu_result_t nu_plugin_terminate(void);
nu_result_t nu_plugin_start(void);
nu_result_t nu_plugin_stop(void);
nu_result_t nu_plugin_update(void);
nu_result_t nu_plugin_fixed_update(void);
nu_result_t nu_plugin_late_update(void);

#endif