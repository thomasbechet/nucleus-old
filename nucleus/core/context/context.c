#include <nucleus/core/context/context.h>

#include <nucleus/core/config/config.h>
#include <nucleus/core/coresystem/event/event.h>
#include <nucleus/core/coresystem/logger/logger.h>
#include <nucleus/core/coresystem/memory/memory.h>
#include <nucleus/core/coresystem/module/module.h>
#include <nucleus/core/coresystem/plugin/plugin.h>
#include <nucleus/core/system/input/input.h>
#include <nucleus/core/system/renderer/renderer.h>
#include <nucleus/core/system/task/task.h>
#include <nucleus/core/system/window/window.h>

typedef enum {
    NU_INIT_STATE_UNINITIALIZED,
    NU_INIT_STATE_INITIALIZED,
    NU_INIT_STATE_STARTED
} nu_init_state_t;

typedef struct {
    nu_init_state_t config;

    nu_init_state_t memory;
    nu_init_state_t logger;
    nu_init_state_t module;
    nu_init_state_t plugin;
    nu_init_state_t event;
    
    nu_init_state_t task;
    nu_init_state_t window;
    nu_init_state_t input;
    nu_init_state_t renderer;
} nu_system_states_t;

typedef struct {
    nu_context_callback_t callback;
    nu_system_states_t states;
    float delta_time;
    bool should_stop;
    nu_timer_t timer;
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

    /* load configuration */
    if (info) _context.callback = info->callback;
    nu_info(NU_LOGGER_NAME, "Loading configuration...");
    if (nu_config_load(_context.callback.config) != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE; }
    _context.states.config = NU_INIT_STATE_STARTED;

    /* initialize core systems */
    nu_info(NU_LOGGER_NAME, "========== Initializing core systems ==========");
    nu_info(NU_LOGGER_NAME, "Initializing core system: memory...");
    if (nu_memory_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.memory = NU_INIT_STATE_INITIALIZED;
    nu_info(NU_LOGGER_NAME, "Initializing core system: logger...");
    if (nu_logger_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.logger = NU_INIT_STATE_INITIALIZED;
    nu_info(NU_LOGGER_NAME, "Initializing core system: module...");
    if (nu_module_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.module = NU_INIT_STATE_INITIALIZED;
    nu_info(NU_LOGGER_NAME, "Initializing core system: plugin...");
    if (nu_plugin_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.plugin = NU_INIT_STATE_INITIALIZED;
    nu_info(NU_LOGGER_NAME, "Initializing core system: event...");
    if (nu_event_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.event = NU_INIT_STATE_INITIALIZED;

    /* initialize systems */
    nu_info(NU_LOGGER_NAME, "============== Initializing systems ===========");
    nu_info(NU_LOGGER_NAME, "Initializing system: task...");
    if (nu_task_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.task = NU_INIT_STATE_INITIALIZED;
    nu_info(NU_LOGGER_NAME, "Initializing system: window...");
    if (nu_window_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.window = NU_INIT_STATE_INITIALIZED;
    nu_info(NU_LOGGER_NAME, "Initializing system: input...");
    if (nu_input_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.input = NU_INIT_STATE_INITIALIZED;
    nu_info(NU_LOGGER_NAME, "Initializing system: renderer...");
    if (nu_renderer_initialize() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.renderer = NU_INIT_STATE_INITIALIZED;

    /* start core systems */
    nu_info(NU_LOGGER_NAME, "============ Starting core systems ============");
    nu_info(NU_LOGGER_NAME, "Starting core system: memory...");
    if (nu_memory_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.memory = NU_INIT_STATE_STARTED;
    nu_info(NU_LOGGER_NAME, "Starting core system: logger...");
    if (nu_logger_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.logger = NU_INIT_STATE_STARTED;
    nu_info(NU_LOGGER_NAME, "Starting core system: module...");
    if (nu_module_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.module = NU_INIT_STATE_STARTED;
    nu_info(NU_LOGGER_NAME, "Starting core system: plugin...");
    if (nu_plugin_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.plugin = NU_INIT_STATE_STARTED;
    nu_info(NU_LOGGER_NAME, "Starting core system: event...");
    if (nu_event_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.event = NU_INIT_STATE_STARTED;

    /* start systems */
    nu_info(NU_LOGGER_NAME, "============== Starting systems ===============");
    nu_info(NU_LOGGER_NAME, "Starting system: task...");
    if (nu_task_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.task = NU_INIT_STATE_STARTED;
    nu_info(NU_LOGGER_NAME, "Starting system: window...");
    if (nu_window_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.window = NU_INIT_STATE_STARTED;
    nu_info(NU_LOGGER_NAME, "Starting system: input...");
    if (nu_input_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.input = NU_INIT_STATE_STARTED;
    nu_info(NU_LOGGER_NAME, "Starting system: renderer...");
    if (nu_renderer_start() != NU_SUCCESS) {nu_context_terminate(); return NU_FAILURE;}
    _context.states.renderer = NU_INIT_STATE_STARTED;

    nu_info(NU_LOGGER_NAME, "===============================================");
    nu_info(NU_LOGGER_NAME, "Running context...");

    return result;
}
static nu_result_t nu_context_terminate(void)
{
    /* stop systems */
    nu_info(NU_LOGGER_NAME, "=============== Stopping systems ==============");
    if (_context.states.renderer == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping system: renderer...");
        nu_renderer_stop();
        _context.states.renderer = NU_INIT_STATE_INITIALIZED;
    }
    if (_context.states.input == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping system: input...");
        nu_input_stop();
        _context.states.input = NU_INIT_STATE_INITIALIZED;
    }
    if (_context.states.window == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping system: window...");
        nu_window_stop();
        _context.states.window = NU_INIT_STATE_INITIALIZED;
    }
    if (_context.states.task == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping system: task...");
        nu_task_stop();
        _context.states.task = NU_INIT_STATE_INITIALIZED;
    }

    /* stop core systems */
    nu_info(NU_LOGGER_NAME, "============= Stopping core systems ===========");
    if (_context.states.event == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping core system: event...");
        nu_event_stop();
        _context.states.event = NU_INIT_STATE_INITIALIZED;
    }
    if (_context.states.plugin == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping core system: plugin...");
        nu_plugin_stop();
        _context.states.plugin = NU_INIT_STATE_INITIALIZED;
    }
    if (_context.states.module == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping core system: module...");
        nu_module_stop();
        _context.states.module = NU_INIT_STATE_INITIALIZED;
    }
    if (_context.states.logger == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping core system: logger...");
        nu_logger_stop();
        _context.states.logger = NU_INIT_STATE_INITIALIZED;
    }
    if (_context.states.memory == NU_INIT_STATE_STARTED) {
        nu_info(NU_LOGGER_NAME, "Stopping core system: memory...");
        nu_memory_stop();
        _context.states.memory = NU_INIT_STATE_INITIALIZED;
    }

    /* terminate systems */
    nu_info(NU_LOGGER_NAME, "============== Terminating systems ============");
    if (_context.states.renderer == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating system: renderer...");
        nu_renderer_terminate();
        _context.states.renderer = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_context.states.input == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating system: input...");
        nu_input_terminate();
        _context.states.input = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_context.states.window == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating system: window...");
        nu_window_terminate();
        _context.states.window = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_context.states.task == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating system: task...");
        nu_task_terminate();
        _context.states.task = NU_INIT_STATE_UNINITIALIZED;
    }

    /* terminate core systems */
    nu_info(NU_LOGGER_NAME, "========= Terminating core systems ============");
    if (_context.states.event == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating core system: event...");
        nu_event_terminate();
        _context.states.event = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_context.states.plugin == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating core system: plugin...");
        nu_plugin_terminate();
        _context.states.plugin = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_context.states.module == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating core system: module...");
        nu_module_terminate();
        _context.states.module = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_context.states.logger == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating core system: logger...");
        nu_logger_terminate();
        _context.states.logger = NU_INIT_STATE_UNINITIALIZED;
    }
    if (_context.states.memory == NU_INIT_STATE_INITIALIZED) {
        nu_info(NU_LOGGER_NAME, "Terminating core system: memory...");
        nu_memory_terminate();
        _context.states.memory = NU_INIT_STATE_UNINITIALIZED;
    }
    nu_info(NU_LOGGER_NAME, "===============================================");

    /* unload configuration */
    nu_info(NU_LOGGER_NAME, "Unloading configuration...");
    if (_context.states.config == NU_INIT_STATE_INITIALIZED) {
        nu_config_unload();
        _context.states.config = NU_INIT_STATE_UNINITIALIZED;
    }

    return NU_SUCCESS;
}

static nu_result_t nu_context_run(void)
{
    const float FIXED_TIMESTEP = 1.0f / 50.0f * 1000.0f;
    const float MAXIMUM_TIMESTEP = 1.0f / 10.0f * 1000.0f;
    float delta, accumulator;

    delta = accumulator = 0.0f;

    /* create timer */
    nu_timer_allocate(&_context.timer);
    nu_timer_start(_context.timer);

    if (_context.callback.start)
        _context.callback.start();

    /* log initial module table */
    nu_module_log();

    while (!_context.should_stop) {
        /* compute delta */
        delta = nu_timer_get_time_elapsed(_context.timer);
        nu_timer_start(_context.timer);

        if (delta > MAXIMUM_TIMESTEP)
            delta = MAXIMUM_TIMESTEP; /* slowing down */

        accumulator += delta;

        /* process window */
        nu_window_update();

        /* process inputs */
        nu_input_update();

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
        nu_renderer_render();
    }

    if (_context.callback.stop) 
        _context.callback.stop();

    /* free resources */
    nu_timer_free(_context.timer);

    return NU_SUCCESS;
}

nu_result_t nu_context_init(const nu_context_init_info_t *info)
{
    nu_result_t result;
    result = NU_SUCCESS;

    result = nu_context_initialize(info);
    if (result != NU_SUCCESS) {
        nu_fatal(NU_LOGGER_NAME, "Failed to initialize nucleus engine.");
        return result;
    }
    result = nu_context_run();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_NAME, "Execution didn't end correctly.");
    }
    result = nu_context_terminate();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_NAME, "Execution didn't terminate correctly.");
    }

#ifdef NU_DEBUG
    nu_info(NU_LOGGER_NAME, "[DEBUG] End process reached.");
#endif

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
void nu_interrupt(const char *id, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_fatal(id, format, args);
    va_end(args);
    exit(NU_FAILURE);
}