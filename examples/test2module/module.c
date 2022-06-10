#include <examples/test2module/module.h>

typedef struct {
    uint32_t mystate;
} nutest2_module_state_t;

static nutest2_module_state_t *state;

static void nutest2_module_load(nu_module_t module, bool hotreload)
{
    (void)module;
    (void)hotreload;
}
static void nutest2_module_unload(nu_module_t module, bool hotreload)
{
    (void)module;
    (void)hotreload;
}

NU_API void nu_module_info(nu_module_info_t *info)
{
    info->name             = NU_TEST2_MODULE_NAME;
    info->callbacks.load   = nutest2_module_load;
    info->callbacks.unload = nutest2_module_unload;
}