#ifndef NU_API_MODULE_H
#define NU_API_MODULE_H

#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>
#include <nucleus/core/utility/platform.h>

NU_DECLARE_HANDLE(nu_module_t);

typedef struct {
    // Called when the module is loaded or hotreloaded. Module operations such as API
    // and persistent state registration can be done here. When hotreloading, the module
    // must recreate non-persistent states.
    void (*load)(nu_module_t module, bool hotreload);
    // Called when the module is unloaded. When hotreloading, the module must unload
    // non-persistent states.
    void (*unload)(nu_module_t module, bool hotreload);
} nu_module_callbacks_t;

typedef struct {
    nu_module_callbacks_t callbacks;
    const char *name;
} nu_module_info_t;

typedef void (*nu_module_info_pfn_t)(nu_module_info_t *info);

typedef struct {
    nu_module_t (*open)(const char *path);
    nu_module_t (*open_static)(nu_module_info_t info);
    nu_result_t (*close)(nu_module_t handle);
    nu_result_t (*hotreload)(nu_module_t handle);
    void (*log)(void);
} nu_module_api_t;

#endif