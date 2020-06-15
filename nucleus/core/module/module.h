#ifndef NU_MODULE_H
#define NU_MODULE_H

#include "../common/common.h"

typedef enum {
    NU_MODULE_TYPE_TASK,
    NU_MODULE_TYPE_WINDOW,
    NU_MODULE_TYPE_RENDERER,
    NU_MODULE_TYPE_PLUGIN
} nu_module_type_t;

typedef struct {
    nu_module_type_t type;
    uint32_t id;
    uint32_t interface_count;
    const char **interfaces;
} nu_module_info_t;

typedef struct {
    nu_module_info_t info;
    void *handle;
} nu_module_t;

NU_API nu_result_t nu_module_load(nu_module_t *module, const char *module_name);
NU_API nu_result_t nu_module_unload(const nu_module_t *module);
NU_API nu_result_t nu_module_load_function(const nu_module_t *module, const char *function_name, nu_pfn_t *function);

#endif