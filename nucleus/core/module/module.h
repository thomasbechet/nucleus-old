#ifndef NU_MODULE_H
#define NU_MODULE_H

#include "../common/common.h"

NU_DECLARE_HANDLE(nu_module_handle_t);

typedef enum {
    NU_MODULE_FLAG_TYPE_TASK      = 1 << 0,
    NU_MODULE_FLAG_TYPE_WINDOW    = 1 << 1,
    NU_MODULE_FLAG_TYPE_INPUT     = 1 << 2,
    NU_MODULE_FLAG_TYPE_RENDERER  = 1 << 3,
    NU_MODULE_FLAG_TYPE_PLUGIN    = 1 << 4
} nu_module_flags_t;

typedef struct {
    const char *name;
    nu_id_t id;
    uint32_t flags;
    const char **plugins;
    uint32_t plugin_count;
} nu_module_info_t;

/* private functions */
nu_result_t nu_module_initialize(void);
nu_result_t nu_module_terminate(void);

/* public functions */
NU_API nu_result_t nu_module_load(nu_module_handle_t *handle, const char *path);
NU_API nu_result_t nu_module_load_function(nu_module_handle_t handle, const char *function_name, nu_pfn_t *function);
NU_API nu_result_t nu_module_get_by_name(nu_module_handle_t *handle, const char *name);
NU_API nu_result_t nu_module_get_by_id(nu_module_handle_t *handle, nu_id_t id);
NU_API nu_id_t nu_module_get_id(nu_module_handle_t handle);
NU_API nu_result_t nu_module_log(void);

#endif