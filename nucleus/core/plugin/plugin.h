#ifndef NU_PLUGIN_H
#define NU_PLUGIN_H

#include "../common/common.h"
#include "../module/module.h"

/* private functions */
nu_result_t nu_plugin_initialize(void);
nu_result_t nu_plugin_terminate(void);
nu_result_t nu_plugin_update(void);
nu_result_t nu_plugin_fixed_update(void);
nu_result_t nu_plugin_late_update(void);

/* public functions */
NU_API nu_result_t nu_plugin_require(nu_module_handle_t module, const char *plugin);

#endif