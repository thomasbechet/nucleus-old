#ifndef NU_TEST_MODULE_H
#define NU_TEST_MODULE_H

#include <nucleus/nucleus.h>

#define NU_TEST_MODULE_NAME "testmodule"

typedef struct {
    void (*run)(void);
} nutest_module_api_t;

NU_API void nu_module_info(nu_module_info_t *info);

#endif