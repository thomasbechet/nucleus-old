#include "context.h"

#include "../system/task.h"
#include "../system/window.h"
#include "../system/renderer.h"

#define NU_CONTEXT_LOG_NAME "[CONTEXT] "

typedef struct {
    bool config;
    bool task;
    bool window;
    bool renderer;
} nu_context_loaded_state_t;

typedef struct {
    bool should_stop;
    nu_context_loaded_state_t loaded;
} nu_context_t;

static nu_context_t _context;

static nu_result_t nu_context_initialize(const nu_init_info_t *info);
static nu_result_t nu_context_terminate(void);

static nu_result_t nu_context_initialize(const nu_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    nu_info(NU_CONTEXT_LOG_NAME"Starting context...\n");

    memset(&_context, 0, sizeof(nu_context_t));
    _context.should_stop     = false;
    _context.loaded.config   = false;
    _context.loaded.task     = false;
    _context.loaded.window   = false;
    _context.loaded.renderer = false;

    /* engine configuration */
    nu_info(NU_CONTEXT_LOG_NAME"Loading configuration...\n");
    nu_config_callback_t callback = info ? info->config_callback : NULL; 
    if (nu_config_load(callback) != NU_SUCCESS) {
        nu_fatal(NU_CONTEXT_LOG_NAME"Failed to configure the engine.\n");
    }
    _context.loaded.config = true;

    /* load task system */
    nu_info(NU_CONTEXT_LOG_NAME"Starting task system...\n");
    result = nu_system_task_load();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.task = true;

    /* load window system */
    nu_info(NU_CONTEXT_LOG_NAME"Starting window system...\n");
    result = nu_system_window_load();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.window = true;

    /* load renderer system */
    nu_info(NU_CONTEXT_LOG_NAME"Starting renderer system...\n");
    result = nu_system_renderer_load();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.renderer = true;

    return result;
}
static nu_result_t nu_context_terminate(void)
{
    if (_context.loaded.renderer) {
        nu_info(NU_CONTEXT_LOG_NAME"Stopping renderer system...\n");
        nu_system_renderer_unload();
        _context.loaded.renderer = false;
    }
    if (_context.loaded.window) {
        nu_info(NU_CONTEXT_LOG_NAME"Stopping window system...\n");
        nu_system_window_unload();
        _context.loaded.window = false;
    }
    if (_context.loaded.task) {
        nu_info(NU_CONTEXT_LOG_NAME"Stopping task system...\n");
        nu_system_task_unload();
        _context.loaded.task = false;
    }
    if (_context.loaded.config) {
        nu_info(NU_CONTEXT_LOG_NAME"Unloading configuration...\n");
        nu_config_unload();
        _context.loaded.config = false;
    }

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

    /* start engine */
    result = nu_context_initialize(info);
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