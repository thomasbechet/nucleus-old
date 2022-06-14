#include <nucleus/nucleus.h>
#include "testmodule/module.h"
#include "test2module/module.h"

typedef struct {
    nu_logger_t logger;
} nu_state_t;

static nu_state_t ctx;

static void fixed_update(void)
{
    // nu_info(ctx.logger, "Update %f", nu_engine_get_delta_time());
    //nu_engine_request_stop();
}

int main(void)
{
    // Initialize nucleus
    nu_initialize_info_t initialize_info = {
        .enable_hotreload   = true,
        .load_engine_config = true
    };
    nu_initialize(&initialize_info);

    // Open modules
    nu_module_open("testmodule");
    nu_module_open("test2module");
    nu_module_log();

    // Log config
    nu_config_log();
    // NU_ASSERT(nu_module_hotreload(module));

    ctx.logger = nu_logger_create("TEST");

    // Run engine
    nu_engine_info_t info;
    memset(&info, 0, sizeof(info));
    info.callbacks.fixed_update = fixed_update;
    NU_ASSERT(nu_engine_run(&info));

    // Terminate framework
    nu_terminate();
    printf("end reached.\n");
    return 0;
}