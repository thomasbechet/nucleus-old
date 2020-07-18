#ifndef NU_MODULE_H
#define NU_MODULE_H

#include "../common/common.h"

typedef enum {
    NU_MODULE_FLAG_TYPE_TASK      = 1 << 0,
    NU_MODULE_FLAG_TYPE_WINDOW    = 1 << 1,
    NU_MODULE_FLAG_TYPE_INPUT     = 1 << 2,
    NU_MODULE_FLAG_TYPE_RENDERER  = 1 << 3,
    NU_MODULE_FLAG_TYPE_PLUGIN    = 1 << 4
} nu_module_flags_t;

typedef struct {
    uint32_t flags;
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