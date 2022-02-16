#ifndef NU_PLUGIN_INTERFACE_H
#define NU_PLUGIN_INTERFACE_H

#include <nucleus/core/coresystem/module/interface.h>

#define NU_PLUGIN_INTERFACE_NAME "nu_plugin_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*fixed_update)(void);
    nu_result_t (*update)(void);
    nu_result_t (*late_update)(void);
} nu_plugin_callbacks_t;

typedef struct {
    nu_result_t (*get_callbacks)(const char*, nu_plugin_callbacks_t*);
    nu_result_t (*get_list)(uint32_t*, const char***);
} nu_plugin_interface_t;

NU_API nu_result_t nu_plugin_require(nu_module_t module, const char *plugin);
NU_API nu_result_t nu_plugin_get_list(nu_module_t module, uint32_t *count, const char ***plugins);

#endif