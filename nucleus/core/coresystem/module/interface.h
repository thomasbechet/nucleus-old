#ifndef NU_MODULE_INTERFACE_H
#define NU_MODULE_INTERFACE_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_module_t);

typedef enum {
    NU_MODULE_FLAG_NONE       = 0x0,
    NU_MODULE_FLAG_HOT_RELOAD = 0x1
} nu_module_flags_t;

typedef struct {
    const char *name;
    uint32_t id;
    uint32_t flags;
    const char **interfaces;
    uint32_t interface_count;
} nu_module_info_t;

NU_API nu_result_t nu_module_load(const char *path, nu_module_t *handle);
NU_API nu_result_t nu_module_get_function(nu_module_t handle, const char *function_name, nu_pfn_t *function);
NU_API nu_result_t nu_module_get_interface(nu_module_t handle, const char *interface_name, void *interface);
NU_API nu_result_t nu_module_get_by_name(const char *name, nu_module_t *handle);
NU_API nu_result_t nu_module_get_by_id(uint32_t id, nu_module_t *handle);
NU_API uint32_t nu_module_get_id(nu_module_t handle);

NU_API nu_result_t nu_module_log(void);

#endif