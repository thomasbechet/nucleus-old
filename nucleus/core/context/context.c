#include "context.h"

#include "../event/event.h"
#include "../plugin/plugin.h"
#include "../system/task.h"
#include "../system/window.h"
#include "../system/renderer.h"

#define NU_CONTEXT_LOGGER_NAME "[CONTEXT] "

typedef struct {
    bool config;
    bool event;
    bool plugin;
    bool task;
    bool window;
    bool input;
    bool renderer;
} nu_context_loaded_state_t;

typedef struct {
    nu_context_loaded_state_t loaded;
    float delta_time;
    bool should_stop;
} nu_context_t;

static nu_context_t _context;

static nu_result_t nu_context_initialize(const nu_init_info_t *info);
static nu_result_t nu_context_terminate(void);

static nu_result_t nu_context_initialize(const nu_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_context, 0, sizeof(nu_context_t));
    _context.should_stop     = false;
    _context.loaded.config   = false;
    _context.loaded.event    = false;
    _context.loaded.plugin   = false;
    _context.loaded.task     = false;
    _context.loaded.window   = false;
    _context.loaded.renderer = false;

    /* engine configuration */
    nu_info(NU_CONTEXT_LOGGER_NAME"Loading configuration...\n");
    nu_config_callback_t callback = info ? info->config_callback : NULL; 
    if (nu_config_load(callback) != NU_SUCCESS) {
        nu_fatal(NU_CONTEXT_LOGGER_NAME"Failed to configure the engine.\n");
    }
    _context.loaded.config = true;
    if (nu_config_get().context.log_config) {
        nu_config_log();
    }

    /* start event system */
    nu_info(NU_CONTEXT_LOGGER_NAME"Initializing event...\n");
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
    nu_info(NU_CONTEXT_LOGGER_NAME"Initializing plugin...\n");
    result = nu_plugin_initialize();
    if (result != NU_SUCCESS) {
        nu_context_terminate();
        return result;
    }
    _context.loaded.plugin = true;

    return result;
}
static nu_result_t nu_context_terminate(void)
{
    if (_context.loaded.plugin) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Terminating plugin...\n");
        nu_plugin_terminate();
        _context.loaded.plugin = false;
    }
    if (_context.loaded.renderer) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping renderer system...\n");
        nu_system_renderer_terminate();
        _context.loaded.renderer = false;
    }
    if (_context.loaded.input) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping input system...\n");
        nu_system_input_terminate();
        _context.loaded.input = false;
    }
    if (_context.loaded.window) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping window system...\n");
        nu_system_window_terminate();
        _context.loaded.window = false;
    }
    if (_context.loaded.task) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Stopping task system...\n");
        nu_system_task_terminate();
        _context.loaded.task = false;
    }
    if (_context.loaded.event) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Terminating event...\n");
        nu_event_terminate();
        _context.loaded.event = false;
    }
    if (_context.loaded.config) {
        nu_info(NU_CONTEXT_LOGGER_NAME"Unloading configuration...\n");
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

    nu_timer_t timer;
    nu_timer_start(&timer);

    nu_plugin_handle_t plugin;
    nu_plugin_load(&plugin, "engine/plugin/nucleus-utility", "nu_utility_plugin_command");
    nu_plugin_load(&plugin, "engine/plugin/nucleus-utility", "nu_utility_plugin_console");

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
            nu_plugin_fixed_update();
        }

        _context.delta_time = delta;

        /* process frame update */
        nu_plugin_update();

        /* process late update */
        nu_plugin_late_update();

        /* process render */
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