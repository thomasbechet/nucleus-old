#include <nucleus/core/core.h>

#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/config/config.h>
#include <nucleus/core/system/engine/engine.h>
#include <nucleus/core/system/event/event.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/module/module.h>

void nu_initialize(void)
{
    // Initialize systems
    nu_allocator_initialize();
    nu_config_initialize();
    nu_logger_initialize();
    nu_event_initialize();
    nu_module_initialize();

    // Register APIs
    nu_allocator_register_api();
    nu_config_register_api();
    nu_logger_register_api();
    nu_event_register_api();
    nu_engine_register_api();
}
void nu_terminate(void)
{
    // Check memory leaks
    nu_allocator_check_memory_leak();
    
    // Terminate systems
    nu_module_terminate();
    nu_event_terminate();
    nu_logger_terminate();
    nu_config_terminate();
    nu_allocator_terminate();
}