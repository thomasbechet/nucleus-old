#include <nucleus/nucleus.h>
#include "testmodule/module.h"
#include "test2module/module.h"

typedef struct {
    nu_logger_t logger;
} nu_state_t;

static nu_state_t ctx;

static void fixed_update(void)
{
    nu_info(ctx.logger, "Update %f", nu_engine_get_delta_time());
}

int main(int argc, char *argv[]) 
{
    // Initialize nucleus
    nu_initialize();

    // Open modules
    nu_module_t module = nu_module_open("testmodule");
    nu_module_open("test2module");
    // nu_module_close(module);
    nu_module_log();

    // Reload module
    nu_config_load("engine/nucleus.ini");
    // NU_ASSERT(nu_module_hotreload(module));

    ctx.logger = nu_logger_create("TEST");

    // Run engine
    nu_engine_info_t info;
    memset(&info, 0, sizeof(info));
    info.callbacks.fixed_update = fixed_update;
    info.auto_hotreload = true;
    NU_ASSERT(nu_engine_run(&info));

    // Terminate framework
    nu_terminate();
    printf("end reached.\n");
    return 0;
}