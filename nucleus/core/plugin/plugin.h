#ifndef NU_PLUGIN_H
#define NU_PLUGIN_H

#include "../common/common.h"
#include "../module/module.h"

NU_DECLARE_HANDLE(nu_plugin_handle_t);

/* private functions */
nu_result_t nu_plugin_initialize(void);
nu_result_t nu_plugin_terminate(void);
nu_result_t nu_plugin_update(void);
nu_result_t nu_plugin_fixed_update(void);
nu_result_t nu_plugin_late_update(void);

/* public functions */
NU_API nu_result_t nu_plugin_load(nu_plugin_handle_t *handle, const char *module, const char *plugin);

#endif