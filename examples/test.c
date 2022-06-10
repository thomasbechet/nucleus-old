#include <nucleus/nucleus.h>
#include "testmodule/module.h"
#include "test2module/module.h"

typedef struct {
    uint32_t it;
} my_api_t;

static my_api_t *s_my_api;

int main(int argc, char *argv[]) 
{
    // Initialize nucleus
    nu_initialize();

    // Open modules
    nu_module_t module = nu_module_open("testmodule");
    nu_module_open("test2module");

    nu_allocator_t a = nu_allocator_create_freelist("test");
    nu_logger_t logger = nu_logger_create("test");

    s_my_api = nu_api_get(my_api_t);

    // Log
    nu_config_load("engine/nucleus.ini");
    nu_config_log();
    nu_module_log();

    // Reload module
    NU_ASSERT(nu_module_hotreload(module));

    // Terminate framework
    nu_terminate();
    printf("end reached.\n");
    return 0;
}