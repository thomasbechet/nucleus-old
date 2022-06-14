#include <examples/testmodule/module.h>

typedef struct {
    nu_logger_t logger;
} nu_test_module_state_t;

static nu_test_module_state_t *ctx;

static void nu_module_load(nu_module_t module, bool hotreload)
{
    ctx = nu_module_get_persitent_state(module, nu_test_module_state_t);
    if (!hotreload) {
        ctx->logger = nu_logger_create("TESTMODULE");
        nu_info(ctx->logger, "Test module loaded !");
    } else {
        nu_info(ctx->logger, "Test module reloaded !");
    }
}
static void nu_module_unload(nu_module_t module, bool hotreload)
{
    (void)module;
    if (!hotreload) {
        nu_info(ctx->logger, "Test module will unload :(");
    } else {
        nu_info(ctx->logger, "Test module will hotreload !");
    }
}

NU_API void nu_module_info(nu_module_info_t *info)
{
    info->name             = NU_TEST_MODULE_NAME;
    info->callbacks.load   = nu_module_load;
    info->callbacks.unload = nu_module_unload;
    info->enable_hotreload = true;
}