#ifndef NU_MODULE_API_H
#define NU_MODULE_API_H

#include <nucleus/core/api/module.h>

NU_API nu_module_t nu_module_open(const char *path);
NU_API nu_module_t nu_module_open_static(nu_module_info_t info);
NU_API nu_result_t nu_module_close(nu_module_t handle);
NU_API nu_result_t nu_module_hotreload(nu_module_t handle);
NU_API nu_result_t nu_module_register_api_(
    nu_module_t handle,
    const char *name,
    size_t size,
    const void *data
);
NU_API void *nu_module_get_persitent_state_(
    nu_module_t handle,
    size_t size
);
NU_API void *nu_module_get_api_(nu_module_t module, const char *name, size_t size);
NU_API void *nu_api_get_(const char *name, size_t size);
NU_API void nu_module_log(void);

#define nu_module_register_api(module, type, papi) \
    nu_module_register_api_(module, #type, sizeof(type), papi)
#define nu_module_get_persitent_state(module, type) \
    (type*)nu_module_get_persitent_state_(module, sizeof(type))
#define nu_module_get_api(module, type) \
    (type*)nu_module_get_api_(module, #type, sizeof(type))
#define nu_api_get(type) \
    (type*)nu_api_get_(#type, sizeof(type))

#endif