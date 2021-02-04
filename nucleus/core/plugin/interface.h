#ifndef NU_PLUGIN_INTERFACE_H
#define NU_PLUGIN_INTERFACE_H

#include "../common/common.h"

/* plugin interface */
#define NU_PLUGIN_INTERFACE_LOADER_NAME "nu_plugin_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*fixed_update)(void);
    nu_result_t (*update)(void);
    nu_result_t (*late_update)(void);
} nu_plugin_interface_t;

typedef nu_result_t (*nu_plugin_interface_loader_pfn_t)(nu_plugin_interface_t*, const char*);

#endif