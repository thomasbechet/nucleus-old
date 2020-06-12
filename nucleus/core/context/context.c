#include "context.h"

#define NU_CONTEXT_LOG_NAME "[CONTEXT] "

typedef struct {
    bool should_stop;
    nu_config_t config;
} nu_context_t;

static nu_context_t _context;

static nu_result_t nu_context_initialize(const nu_config_t *config)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_context, 0, sizeof(nu_context_t));
    _context.should_stop = false;
    _context.config = *config;

    /* load task system */
    result = nu_system_task_load();
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load window system */
    result = nu_system_window_load(config->window_api);
    if (result != NU_SUCCESS) {
        nu_system_task_unload();
        return result;
    }

    /* load renderer system */
    result = nu_system_renderer_load(config->renderer_api);
    if (result != NU_SUCCESS) {
        nu_system_window_unload();
        nu_system_task_unload();
        return result;
    }

    return result;
}
static nu_result_t nu_context_terminate(void)
{
    nu_system_renderer_unload();
    nu_system_window_unload();
    nu_system_task_unload();

    return NU_SUCCESS;
}
static nu_result_t nu_context_run(void)
{
    while (!_context.should_stop) {
        nu_system_window_update();
        nu_system_renderer_render();
    }

    return NU_SUCCESS;
}

nu_result_t nu_init(const nu_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* engine configuration */
    nu_config_t config;
    memset(&config, 0, sizeof(nu_config_t));
    result = nu_config_default(&config);
    if (result != NU_SUCCESS) {
        nu_fatal(NU_CONTEXT_LOG_NAME"Failed to apply default configuration.\n");
        return result;
    }

    if (info) {
        if (info->config_callback) {
            result = info->config_callback(&config);
            if (result != NU_SUCCESS) {
                nu_fatal(NU_CONTEXT_LOG_NAME"Failed to apply custom configuration.\n");
                return result;
            }
        }
    }

    /* start engine */
    result = nu_context_initialize(&config);
    if (result != NU_SUCCESS) {
        nu_fatal(NU_CONTEXT_LOG_NAME"Failed to initialize nucleus engine.\n");
        return result;
    }
    result = nu_context_run();
    if (result != NU_SUCCESS) {
        nu_warning(NU_CONTEXT_LOG_NAME"Nucleus engine didn't end correctly.\n");
    }
    result = nu_context_terminate();
    if (result != NU_SUCCESS) {
        nu_warning(NU_CONTEXT_LOG_NAME"Nucleus engine didn't terminate correctly.\n");
    }

    /* memory check */
#ifdef NU_DEBUG
    if (nu_memory_total_alloc() != nu_memory_total_free()) {
        nu_warning(NU_CONTEXT_LOG_NAME"Memory leak detected (alloc: %llu free: %llu).\n", nu_memory_total_alloc(), nu_memory_total_free());
    } else {
        nu_info(NU_CONTEXT_LOG_NAME"No memory leak detected (alloc: %llu free: %llu).\n", nu_memory_total_alloc(), nu_memory_total_free());
    }
#endif

    nu_info(NU_CONTEXT_LOG_NAME"End process reached.\n");

    return NU_SUCCESS;
}

nu_result_t nu_context_request_stop(void)
{
    _context.should_stop = true;

    return NU_SUCCESS;
}
nu_config_t nu_context_get_config(void)
{
    return _context.config;
}