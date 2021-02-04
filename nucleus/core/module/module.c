#include "module.h"

#include "../memory/memory.h"
#include "../logger/logger.h"

#define NU_LOGGER_MODULE_NAME "[MODULE] "
#define NU_MODULE_GET_INFO_NAME "nu_module_get_info"

#if defined(NU_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined(NU_PLATFORM_UNIX)
    #include <libgen.h>
#endif

#define MAX_MODULE_COUNT 64
#define MAX_MODULE_PATH_SIZE 256

typedef struct {
    nu_module_info_t info;
    char path[MAX_MODULE_PATH_SIZE];
    void *handle;
} nu_module_t;

typedef struct {
    nu_module_t modules[MAX_MODULE_COUNT];
    uint32_t module_count;
} nu_module_data_t;

static nu_module_data_t _data;

static nu_result_t load_function(const nu_module_t *module, const char *function_name, nu_pfn_t *function)
{
#if defined(NU_PLATFORM_WINDOWS)
    UINT old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    *function = (nu_pfn_t)GetProcAddress((HMODULE)module->handle, function_name);
    SetErrorMode(old_mode);
#elif defined(NU_PLATFORM_UNIX)
    *function = (nu_pfn_t)dlsym(module->handle, function_name);
#endif

    if (!*function) {
        nu_warning(NU_LOGGER_MODULE_NAME"Failed to get function named '%s'.\n", function_name);
        *function = NULL;
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
static nu_result_t unload_module(const nu_module_t *module)
{
#if defined(NU_PLATFORM_WINDOWS)
    UINT old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    FreeLibrary((HMODULE)module->handle);
    SetErrorMode(old_mode);
#elif defined(NU_PLATFORM_UNIX)
    dlclose(module->handle);
#endif

    return NU_SUCCESS;
}
static nu_result_t load_module(nu_module_t *module, const char *filename)
{
    /* reset memory */
    memset(module, 0, sizeof(nu_module_t));

    /* loading module */
#if defined(NU_PLATFORM_WINDOWS)
    char dir[MAX_MODULE_PATH_SIZE];
    char fname[128];
    char path[MAX_MODULE_PATH_SIZE];
    _splitpath_s(filename, NULL, 0, dir, sizeof(dir), fname, sizeof(fname), NULL, 0);
    #if defined(__MINGW32__)
        nu_snprintf(path, MAX_MODULE_PATH_SIZE, "%slib%s.dll", dir, fname);
    #else
        nu_snprintf(path, MAX_MODULE_PATH_SIZE, "%s%s.dll", dir, fname);
    #endif
    module->handle = LoadLibraryA(path);
#elif defined(NU_PLATFORM_UNIX)
    char *dir, *fname;
    char path[MAX_MODULE_PATH_SIZE];
    dir = dirname(filename);
    fname = basename(filename);
    nu_snprintf(path, MAX_MODULE_PATH_SIZE, "%slib%s.so", dir, fname);
    module->handle = dlopen(path, RTLD_LAZY);
    free(dir);
    free(fname);
#endif

    if (!module->handle) {
        nu_warning(NU_LOGGER_MODULE_NAME"Failed to load module '%s'.\n", filename);
        return NU_FAILURE;
    }

    /* copy path */
    strncpy(module->path, filename, MAX_MODULE_PATH_SIZE);

    /* load module info */
    nu_result_t (*module_get_info)(nu_module_info_t*);
    if (load_function(module, NU_MODULE_GET_INFO_NAME, (nu_pfn_t*)&module_get_info) != NU_SUCCESS) {
        nu_warning(NU_LOGGER_MODULE_NAME"'%s' function is required to load the module '%s'.\n", NU_MODULE_GET_INFO_NAME, filename);
        unload_module(module);
        return NU_FAILURE;
    }

    if (module_get_info(&module->info) != NU_SUCCESS) {
        nu_warning(NU_LOGGER_MODULE_NAME"Failed to retrieve info from module '%s'.\n", filename);
        unload_module(module);
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

nu_result_t nu_module_initialize(void)
{
    /* make all handle null */
    for (uint32_t i = 0; i < MAX_MODULE_COUNT; i++) {
        _data.modules[i].handle = NULL;
    }
    _data.module_count = 0;

    return NU_SUCCESS;
}
nu_result_t nu_module_terminate(void)
{
    /* unload all module */
    for (uint32_t i = 0; i < _data.module_count; i++) {
        if (_data.modules[i].handle) {
            unload_module(&_data.modules[i]);
        }
    }

    return NU_SUCCESS;
}

nu_result_t nu_module_load(nu_module_handle_t *handle, const char *path)
{
    /* error check */
    if (_data.module_count >= MAX_MODULE_COUNT) return NU_FAILURE;

    /* load module */
    if (load_module(&_data.modules[_data.module_count], path) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    /* save id */
    NU_HANDLE_SET_ID(*handle, _data.module_count++);

    return NU_SUCCESS;
}
nu_result_t nu_module_load_function(nu_module_handle_t handle, const char *function_name, nu_pfn_t *function)
{
    uint32_t id = NU_HANDLE_GET_ID(handle);
    return load_function(&_data.modules[id], function_name, function);
}
nu_result_t nu_module_get_by_name(nu_module_handle_t *handle, const char *name)
{
    for (uint32_t i = 0; i < _data.module_count; i++) {
        if (NU_MATCH(_data.modules[i].info.name, name)) {
            NU_HANDLE_SET_ID(*handle, i);
            return NU_SUCCESS;
        }
    }

    return NU_FAILURE;
}
nu_result_t nu_module_get_by_id(nu_module_handle_t *handle, nu_id_t id)
{
    for (uint32_t i = 0; i < _data.module_count; i++) {
        if (_data.modules[i].info.id == id) {
            NU_HANDLE_SET_ID(*handle, i);
            return NU_SUCCESS;
        }
    }

    return NU_FAILURE;
}
nu_id_t nu_module_get_id(nu_module_handle_t handle)
{
    uint32_t id = NU_HANDLE_GET_ID(handle);
    return _data.modules[id].info.id;
}

#define MAX_MODULE_FLAG_COUNT 5
#define HEADER_NAME    "NAME"
#define HEADER_ID      "ID"
#define HEADER_FLAGS   "FLAGS"
#define HEADER_PLUGINS "PLUGINS"

static void module_get_line_count_with_flags(
    const nu_module_t *module, char flags[MAX_MODULE_FLAG_COUNT][128], uint32_t n, uint32_t *flag_count, uint32_t *count
)
{
    *flag_count = 0;
    if (module->info.flags & NU_MODULE_FLAG_TYPE_TASK)     {strncpy(flags[(*flag_count)], "task",     n); (*flag_count)++;}
    if (module->info.flags & NU_MODULE_FLAG_TYPE_WINDOW)   {strncpy(flags[(*flag_count)], "window",   n); (*flag_count)++;}
    if (module->info.flags & NU_MODULE_FLAG_TYPE_INPUT)    {strncpy(flags[(*flag_count)], "input",    n); (*flag_count)++;}
    if (module->info.flags & NU_MODULE_FLAG_TYPE_RENDERER) {strncpy(flags[(*flag_count)], "renderer", n); (*flag_count)++;}
    if (module->info.flags & NU_MODULE_FLAG_TYPE_PLUGIN)   {strncpy(flags[(*flag_count)], "plugin",   n); (*flag_count)++;}
    *count = NU_MAX(1, NU_MAX(module->info.plugin_count, *flag_count));
}
static void print_module_line_at_index(
    const nu_module_t *module, char flags[MAX_MODULE_FLAG_COUNT][128], uint32_t flag_count, uint32_t index,
    char *name, char *id, char *flag, char *plugin, uint32_t n
)
{
    /* set default value */
    strncpy(name,   "", n);
    strncpy(id,     "", n);
    strncpy(flag,   "", n);
    strncpy(plugin, "", n);

    /* name + id */
    if (index == 0) {
        strncpy(name, module->info.name, n);
        nu_snprintf(id, n, "0x%x", module->info.id);
    }

    /* flag */
    if (index < flag_count) strncpy(flag, flags[index], n);

    /* plugin */
    if (index == 0 && module->info.plugin_count == 0) {
        strncpy(plugin, "none", n);
    } else if (index < module->info.plugin_count) {
        strncpy(plugin, module->info.plugins[index], n);
    }
}
static void compute_max(
    const nu_module_t *modules, uint32_t module_count,
    uint32_t *max_name, uint32_t *max_id, uint32_t *max_flag, uint32_t *max_plugin
)
{
    *max_name   = strlen(HEADER_NAME);
    *max_id     = strlen(HEADER_ID);
    *max_flag   = strlen(HEADER_FLAGS);
    *max_plugin = strlen(HEADER_PLUGINS);
    for (uint32_t m = 0; m < module_count; m++) {
        char flags[MAX_MODULE_FLAG_COUNT][128];
        uint32_t flag_count, count;
        module_get_line_count_with_flags(&modules[m], flags, 128, &flag_count, &count);
        for (uint32_t i = 0; i < count; i++) {
            char name[128];
            char id[128];
            char flag[128];
            char plugin[128];
            print_module_line_at_index(&modules[m], flags, flag_count, i, name, id, flag, plugin, 128);
            *max_name   = NU_MAX(*max_name, strlen(name));
            *max_id     = NU_MAX(*max_id, strlen(id));
            *max_flag   = NU_MAX(*max_flag, strlen(flag));
            *max_plugin = NU_MAX(*max_plugin, strlen(plugin));
        }
    }
}
static void log_transition_line(uint32_t max_name, uint32_t max_id, uint32_t max_flag, uint32_t max_plugin)
{
    nu_info("+");
    for (uint32_t i = 0; i < max_name + 2; i++)   nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_id + 2; i++)     nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_flag + 2; i++)   nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_plugin + 2; i++) nu_info("-");
    nu_info("+\n");
}
static void log_line(
    uint32_t max_name, uint32_t max_id, uint32_t max_flag, uint32_t max_plugin,
    const char *name, const char *id, const char *flag, const char *plugin
)
{
    uint32_t name_len = strlen(name);
    uint32_t id_len = strlen(id);
    uint32_t flag_len = strlen(flag);
    uint32_t plugin_len = strlen(plugin);
    nu_info("| %s", name);
    for (uint32_t i = 0; i < (max_name - name_len + 1); i++) nu_info(" ");
    nu_info("| %s", id);
    for (uint32_t i = 0; i < (max_id - id_len + 1); i++) nu_info(" ");
    nu_info("| %s", flag);
    for (uint32_t i = 0; i < (max_flag - flag_len + 1); i++) nu_info(" ");
    nu_info("| %s", plugin);
    for (uint32_t i = 0; i < (max_plugin - plugin_len + 1); i++) nu_info(" ");
    nu_info("|\n");
}
nu_result_t nu_module_log(void)
{
    /* compute max */
    uint32_t max_name, max_id, max_flag, max_plugin;
    compute_max(_data.modules, _data.module_count, &max_name, &max_id, &max_flag, &max_plugin);

    /* display header */
    log_transition_line(max_name, max_id, max_flag, max_plugin);
    log_line(max_name, max_id, max_flag, max_plugin, HEADER_NAME, HEADER_ID, HEADER_FLAGS, HEADER_PLUGINS);

    /* display */
    log_transition_line(max_name, max_id, max_flag, max_plugin);
    for (uint32_t i = 0; i < _data.module_count; i++) {
        const nu_module_t *module = &_data.modules[i];
        char flags[MAX_MODULE_FLAG_COUNT][128];
        uint32_t flag_count, count;
        module_get_line_count_with_flags(module, flags, 128, &flag_count, &count);

        /* show line */
        for (uint32_t l = 0; l < count; l++) {
            char name[128];
            char id[128];
            char flag[128];
            char plugin[128];
            print_module_line_at_index(module, flags, flag_count, l, name, id, flag, plugin, 128);
            log_line(max_name, max_id, max_flag, max_plugin, name, id, flag, plugin);
        }

        log_transition_line(max_name, max_id, max_flag, max_plugin);
    }

    return NU_SUCCESS;
}