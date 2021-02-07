#ifndef NU_MACRO_H
#define NU_MACRO_H

#include "platform.h"

#include <stdint.h>

/* min/max */
#define NU_MIN(a,b) (((a)<(b))?(a):(b))
#define NU_MAX(a,b) (((a)>(b))?(a):(b))
#define NU_MATCH(a,b) (strcmp(a,b)==0)

/* handle */
#define NU_DECLARE_HANDLE(name) struct name##__ { int unused; }; \
                                typedef struct name##__ *name
#ifdef NU_CXX
    #define NU_NULL_HANDLE nullptr
    #define NU_HANDLE_SET_ID(handle, id) handle = static_cast<decltype(handle)>((void*)(uintptr_t)(id + 1))
    #define NU_HANDLE_GET_ID(handle) ({uint32_t id; id = (uintptr_t)(handle); (id - 1);})
#else
    #define NU_NULL_HANDLE NULL
    #define NU_HANDLE_SET_ID(handle, id) handle = (void*)(uintptr_t)(id + 1)
    #define NU_HANDLE_GET_ID(handle) ({uint32_t id; id = (uintptr_t)(handle); (id - 1);})
#endif

/* module */
#define NU_MODULE_LOAD_INTERFACE(MODULE_NAME, INTERFACE_NAME, INTERFACE) ({\
    nu_module_handle_t module; \
    nu_result_t status = NU_FAILURE; \
    if (nu_module_get_by_name(&module, MODULE_NAME) == NU_SUCCESS) { \
        status = nu_module_load_interface(module, INTERFACE_NAME, INTERFACE); \
    } \
    status; \
})

#endif