#include "context.h"

#include "../event/event.h"
#include "../plugin/plugin.h"
#include "../system/task/task.h"
#include "../system/window/window.h"
#include "../system/renderer/renderer.h"
#include "../system/input/input.h"
#include "../logger/logger.h"
#include "../memory/memory.h"

#define NU_CONTEXT_LOGGER_NAME "[SYSTEM] "

typedef struct {
    bool config;
    bool memory;
    bool logger;
    bool module;
    bool event;
    bool plugin;
    bool task;
    bool window;
    bool input;
    bool renderer;
} nu_context_loaded_state_t;

typedef struct {
    nu_context_callback_t callback;
    nu_context_loaded_state_t loaded;
    float delta_time;
    bool should_stop;
} nu_context_data_t;

static nu_context_data_t _context;

static nu_result_t nu_context_initialize(const nu_context_init_info_t *info);
static nu_result_t nu_context_terminate(void);

static nu_result_t nu_context_initialize(const nu_context_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_context, 0, sizeof(nu_context_data_t));
    _context.should_stop     = false;

    _context.loaded.config   = false;
    _context.loaded.memory   = false;
    _context.loaded.logger   = false;
    _context.loaded.module   = false;
    _context.loaded.plugin   = false;
    _context.loaded.event    = false;
    _context.loaded.task     = false;
    _context.loaded.window   = false;
    _context.loaded.renderer = false;

    /* load configuration */
    if (info) {
        _context.callback = info->callback;
    }
    if (nu_config_load(_context.callback.config) != NU_SUCCESS) {
        nu_context_terminate();
        return NU_FAILURE;
    }
    _context.loaded.config = true;

    /* initialize memory system */
    result = nu_memory_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return NU_FAILURE;
    }
    _context.loaded.memory = true;

    /* initialize logger system */
    result = nu_logger_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.logger = true;
    
    /* log core configuration */
    if (nu_config_get().context.log_config) {
        nu_config_log();
    }

    /* initialize module system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting module...\n");
    result = nu_module_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.module = true;

    /* initialize event system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting event...\n");
    result = nu_event_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.event = true;

    /* start task system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting task system...\n");
    result = nu_system_task_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.task = true;

    /* start window system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting window system...\n");
    result = nu_system_window_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.window = true;

    /* start input system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting input system...\n");
    result = nu_system_input_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.input = true;

    /* start renderer system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting renderer system...\n");
    result = nu_system_renderer_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.renderer = true;

    /* start plugin system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Starting plugins...\n");
    result = nu_plugin_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.plugin = true;

    nu_info(NU_CONTEXT_LOGGER_NAME"Running context...\n");

    return result;
}
static nu_result_t nu_context_terminate(void)
{
    /* terminate plugin system */
    if (_context.loaded.plugin) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping plugins...\n");
        nu_plugin_terminate();
        _context.loaded.plugin = false;
    }

    /* terminate renderer system */
    if (_context.loaded.renderer) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping renderer system...\n");
        nu_system_renderer_terminate();
        _context.loaded.renderer = false;
    }

    /* terminate input system */
    if (_context.loaded.input) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping input system...\n");
        nu_system_input_terminate();
        _context.loaded.input = false;
    }

    /* terminate window system */
    if (_context.loaded.window) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping window system...\n");
        nu_system_window_terminate();
        _context.loaded.window = false;
    }

    /* terminate task system */
    if (_context.loaded.task) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping task system...\n");
        nu_system_task_terminate();
        _context.loaded.task = false;
    }

    /* terminate event system */
    if (_context.loaded.event) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping event...\n");
        nu_event_terminate();
        _context.loaded.event = false;
    }

    /* terminate module system */
    if (_context.loaded.module) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping module...\n");
        nu_module_terminate();
        _context.loaded.module = false;
    }

    /* terminate logger system */
    if (_context.loaded.logger) {
        nu_logger_terminate();
        _context.loaded.logger = false;
    }

    /* terminate memory system */
    if (_context.loaded.memory) {
        nu_memory_terminate();
        _context.loaded.memory = false;
    }

    /* unload configuration */
    if (_context.loaded.config) {
        nu_config_unload();
        _context.loaded.config = false;
    }

    return NU_SUCCESS;
}

static nu_result_t nu_context_run(void)
{
    const float FIXED_TIMESTEP = 1.0f / 50.0f * 1000.0f;
    const float MAXIMUM_TIMESTEP = 1.0f / 10.0f * 1000.0f;
    float delta, accumulator;

    delta = accumulator = 0.0f;

    nu_timer_t timer;
    nu_timer_start(&timer);

    if (_context.callback.start)
        _context.callback.start();

    nu_module_log();

    while (!_context.should_stop) {
        /* compute delta */
        delta = nu_timer_get_time_elapsed(&timer);
        nu_timer_start(&timer);

        if (delta > MAXIMUM_TIMESTEP)
            delta = MAXIMUM_TIMESTEP; /* slowing down */

        accumulator += delta;

        /* process window */
        nu_system_window_update();

        /* process inputs */
        nu_system_input_update();

        /* dispatch events */
        nu_event_dispatch_all();

        _context.delta_time = FIXED_TIMESTEP;
        while (accumulator >= FIXED_TIMESTEP) {
            accumulator -= FIXED_TIMESTEP;

            /* process fixed update */
            if (_context.callback.fixed_update) 
                _context.callback.fixed_update();
            nu_plugin_fixed_update();
        }

        _context.delta_time = delta;

        /* process frame update */
        if (_context.callback.update) 
            _context.callback.update();
        nu_plugin_update();

        /* process late update */
        if (_context.callback.late_update) 
            _context.callback.late_update();
        nu_plugin_late_update();

        /* process render */
        nu_system_renderer_render();
    }

    if (_context.callback.stop) 
        _context.callback.stop();

    return NU_SUCCESS;
}

nu_result_t nu_context_init(const nu_context_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* start engine */
    result = nu_context_initialize(info);
    if (result != NU_SUCCESS) {
        nu_fatal(NU_CONTEXT_LOGGER_NAME"Failed to initialize nucleus engine.\n");
        return result;
    }
    result = nu_context_run();
    if (result != NU_SUCCESS) {
        nu_warning(NU_CONTEXT_LOGGER_NAME"Nucleus engine didn't end correctly.\n");
    }
    result = nu_context_terminate();
    if (result != NU_SUCCESS) {
        nu_warning(NU_CONTEXT_LOGGER_NAME"Nucleus engine didn't terminate correctly.\n");
    }

    /* memory check */
#ifdef NU_DEBUG
    if (nu_memory_total_alloc() != nu_memory_total_free()) {
        nu_warning(NU_CONTEXT_LOGGER_NAME"Memory leak detected (alloc: %llu free: %llu).\n", nu_memory_total_alloc(), nu_memory_total_free());
    } else {
        nu_info(NU_CONTEXT_LOGGER_NAME"No memory leak detected (alloc: %llu free: %llu).\n", nu_memory_total_alloc(), nu_memory_total_free());
    }
#endif

    nu_info(NU_CONTEXT_LOGGER_NAME"End process reached.\n");

    return NU_SUCCESS;
}
nu_result_t nu_context_request_stop(void)
{
    _context.should_stop = true;
    return NU_SUCCESS;
}
float nu_context_get_delta_time(void)
{
    return _context.delta_time;
}
void nu_interrupt(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_vlog(NU_FATAL, format, args);
    va_end(args);
    exit(NU_FAILURE);
}