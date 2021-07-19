#include <nucleus/core/coresystem/module/module.h>

#include <nucleus/core/coresystem/memory/memory.h>
#include <nucleus/core/coresystem/logger/logger.h>
#include <nucleus/core/utils/indexed_array.h>

#define NU_MODULE_GET_INFO_NAME      "nu_module_get_info"
#define NU_MODULE_GET_INTERFACE_NAME "nu_module_get_interface"

typedef nu_result_t (*nu_module_info_loader_pfn_t)(nu_module_info_t*);
typedef nu_result_t (*nu_module_interface_loader_pfn_t)(const char*, void*);

#if defined(NU_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined(NU_PLATFORM_UNIX)
    #include <libgen.h>
    #include <dlfcn.h>
#endif

#undef interface

#define MAX_MODULE_PATH_SIZE 256

typedef struct {
    nu_module_info_t info;
    char path[MAX_MODULE_PATH_SIZE];
    void *handle;
    nu_module_interface_loader_pfn_t interface_loader;
} nu_module_data_t;

typedef struct {
    nu_indexed_array_t modules;
} nu_system_data_t;

static nu_system_data_t _system;

static nu_result_t load_function(const nu_module_data_t *module, const char *function_name, nu_pfn_t *function)
{
#if defined(NU_PLATFORM_WINDOWS)
    UINT old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    *function = (nu_pfn_t)GetProcAddress((HMODULE)module->handle, function_name);
    SetErrorMode(old_mode);
#elif defined(NU_PLATFORM_UNIX)
    *(void**)(&(*function)) = dlsym(module->handle, function_name);
#endif

    if (!*function) {
        nu_core_log(NU_WARNING, "Failed to get function named '%s'.\n", function_name);
        *function = NULL;
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
static nu_result_t unload_module(const nu_module_data_t *module)
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
static nu_result_t load_module(const char *filename, nu_module_data_t *module)
{
    /* reset memory */
    memset(module, 0, sizeof(nu_module_data_t));

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
    char *tdir, *dir, *fname, *tfname;
    char path[MAX_MODULE_PATH_SIZE];
    tdir = strdup(filename); /* TODO: replace with nu_string_t */
    tfname = strdup(filename);
    dir = dirname(tdir);
    fname = basename(tfname);
    nu_snprintf(path, MAX_MODULE_PATH_SIZE, "%s/lib%s.so", dir, fname);
    free(tdir);
    free(tfname);
    module->handle = dlopen(path, RTLD_LAZY);
#endif

    if (!module->handle) {
        nu_core_log(NU_WARNING, "Failed to load module '%s'.\n", filename);
        return NU_FAILURE;
    }

    /* copy path */
    strncpy(module->path, filename, MAX_MODULE_PATH_SIZE - 1);

    /* load module info */
    nu_module_info_loader_pfn_t module_get_info;
    if (load_function(module, NU_MODULE_GET_INFO_NAME, (nu_pfn_t*)&module_get_info) != NU_SUCCESS) {
        nu_core_log(NU_WARNING, "'%s' function is required to load the module '%s'.\n", NU_MODULE_GET_INFO_NAME, filename);
        unload_module(module);
        return NU_FAILURE;
    }

    if (module_get_info(&module->info) != NU_SUCCESS) {
        nu_core_log(NU_WARNING, "Failed to retrieve info from module '%s'.\n", filename);
        unload_module(module);
        return NU_FAILURE;
    }

    /* load interface loader */
    if (module->info.interface_count > 0) {
        if (load_function(module, NU_MODULE_GET_INTERFACE_NAME, (nu_pfn_t*)&module->interface_loader) != NU_SUCCESS) {
            nu_core_log(NU_WARNING, "Module '%s' has interfaces but no interface loader.\n", module->info.name);
            unload_module(module);
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}

nu_result_t nu_module_initialize(void)
{
    /* allocate module array */
    nu_indexed_array_allocate(sizeof(nu_module_data_t), &_system.modules);

    return NU_SUCCESS;
}
nu_result_t nu_module_terminate(void)
{
    /* unload all module */
    uint32_t size = nu_indexed_array_get_size(_system.modules);
    nu_module_data_t *data = nu_indexed_array_get_data(_system.modules);
    for (uint32_t i = 0; i < size; i++) {
        unload_module(&data[i]);
    }
    nu_indexed_array_clear(_system.modules);

    /* free resources */
    nu_indexed_array_free(_system.modules);

    return NU_SUCCESS;
}
nu_result_t nu_module_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nu_module_stop(void)
{
    return NU_SUCCESS;
}

nu_result_t nu_module_load(const char *path, nu_module_t *handle)
{
    /* load module */
    nu_module_data_t module;
    if (load_module(path, &module) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    /* save id */
    uint32_t id;
    nu_indexed_array_add(_system.modules, &module, &id);
    NU_HANDLE_SET_ID(*handle, id);

    return NU_SUCCESS;
}
nu_result_t nu_module_load_function(nu_module_t handle, const char *function_name, nu_pfn_t *function)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    return load_function((nu_module_data_t*)nu_indexed_array_get(_system.modules, id), function_name, function);
}
nu_result_t nu_module_load_interface(nu_module_t handle, const char *interface_name, void *interface)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    nu_module_data_t *module = (nu_module_data_t*)nu_indexed_array_get(_system.modules, id);
    return module->interface_loader(interface_name, interface);
}
static inline bool nu_module_find_by_name(const void *user, const void *object)
{
    const nu_module_data_t *module = (const nu_module_data_t*)object;
    return NU_MATCH(module->info.name, (const char*)user);
}
nu_result_t nu_module_get_by_name(const char *name, nu_module_t *handle)
{
    uint32_t id;
    if (nu_indexed_array_find_id(_system.modules, nu_module_find_by_name, name, &id)) {
        NU_HANDLE_SET_ID(*handle, id);
        return NU_SUCCESS;
    }
    return NU_FAILURE;
}
static inline bool nu_module_find_by_id(const void *user, const void *object)
{
    const nu_module_data_t *module = (const nu_module_data_t*)object;
    return module->info.id == *(const uint32_t*)user;
}
nu_result_t nu_module_get_by_id(uint32_t id, nu_module_t *handle)
{
    uint32_t array_id;
    if (nu_indexed_array_find_id(_system.modules, nu_module_find_by_id, &id, &array_id)) {
        NU_HANDLE_SET_ID(*handle, array_id);
        return NU_SUCCESS;
    }
    return NU_FAILURE;
}
uint32_t nu_module_get_id(nu_module_t handle)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    nu_module_data_t *module = (nu_module_data_t*)nu_indexed_array_get(_system.modules, id);
    return module->info.id;
}

#define MAX_MODULE_FLAG_COUNT 5
#define HEADER_NAME       "NAME"
#define HEADER_ID         "ID"
#define HEADER_FLAGS      "FLAGS"
#define HEADER_INTERFACES "INTERFACES"
#define HEADER_PLUGINS    "PLUGINS"

static void module_get_line_count_with_flags(
    const nu_module_data_t *module, char flags[MAX_MODULE_FLAG_COUNT][128], uint32_t n, uint32_t *flag_count, uint32_t *count
)
{
    *flag_count = 0;
    if (module->info.flags == 0x0) {strncpy(flags[(*flag_count)], "none", n); (*flag_count)++;}
    *count = NU_MAX(1, NU_MAX(NU_MAX(module->info.interface_count, module->info.plugin_count), *flag_count));
}
static void print_module_line_at_index(
    const nu_module_data_t *module, char flags[MAX_MODULE_FLAG_COUNT][128], uint32_t flag_count, uint32_t index,
    char *name, char *id, char *flag, char *interface, char *plugin, uint32_t n
)
{
    /* set default value */
    strncpy(name,      "", n);
    strncpy(id,        "", n);
    strncpy(flag,      "", n);
    strncpy(interface, "", n);
    strncpy(plugin,    "", n);

    /* name + id */
    if (index == 0) {
        strncpy(name, module->info.name, n);
        nu_snprintf(id, n, "0x%x", module->info.id);
    }

    /* flag */
    if (index < flag_count) strncpy(flag, flags[index], n);

    /* interface */
    if (index == 0 && module->info.interface_count == 0) {
        strncpy(interface, "none", n);
    } else if (index < module->info.interface_count) {
        strncpy(interface, module->info.interfaces[index], n);
    }

    /* plugin */
    if (index == 0 && module->info.plugin_count == 0) {
        strncpy(plugin, "none", n);
    } else if (index < module->info.plugin_count) {
        strncpy(plugin, module->info.plugins[index], n);
    }
}
static void compute_max(
    const nu_module_data_t *modules, uint32_t module_count,
    uint32_t *max_name, uint32_t *max_id, uint32_t *max_flag, uint32_t *max_interface, uint32_t *max_plugin
)
{
    *max_name      = strlen(HEADER_NAME);
    *max_id        = strlen(HEADER_ID);
    *max_flag      = strlen(HEADER_FLAGS);
    *max_interface = strlen(HEADER_INTERFACES);
    *max_plugin    = strlen(HEADER_PLUGINS);
    for (uint32_t m = 0; m < module_count; m++) {
        char flags[MAX_MODULE_FLAG_COUNT][128];
        uint32_t flag_count, count;
        module_get_line_count_with_flags(&modules[m], flags, 128, &flag_count, &count);
        for (uint32_t i = 0; i < count; i++) {
            char name[128];
            char id[128];
            char flag[128];
            char interface[128];
            char plugin[128];
            print_module_line_at_index(&modules[m], flags, flag_count, i, name, id, flag, interface, plugin, 128);
            *max_name      = NU_MAX(*max_name, strlen(name));
            *max_id        = NU_MAX(*max_id, strlen(id));
            *max_flag      = NU_MAX(*max_flag, strlen(flag));
            *max_interface = NU_MAX(*max_interface, strlen(interface));
            *max_plugin    = NU_MAX(*max_plugin, strlen(plugin));
        }
    }
}
static void log_transition_line(uint32_t max_name, uint32_t max_id, uint32_t max_flag, uint32_t max_interface, uint32_t max_plugin)
{
    nu_info("+");
    for (uint32_t i = 0; i < max_name + 2; i++)   nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_id + 2; i++)     nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_flag + 2; i++)   nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_interface + 2; i++)   nu_info("-");
    nu_info("+");
    for (uint32_t i = 0; i < max_plugin + 2; i++) nu_info("-");
    nu_info("+\n");
}
static void log_line(
    uint32_t max_name, uint32_t max_id, uint32_t max_flag, uint32_t max_interface, uint32_t max_plugin,
    const char *name, const char *id, const char *flag, const char *interface, const char *plugin
)
{
    uint32_t name_len      = strlen(name);
    uint32_t id_len        = strlen(id);
    uint32_t flag_len      = strlen(flag);
    uint32_t interface_len = strlen(interface);
    uint32_t plugin_len    = strlen(plugin);
    nu_info("| %s", name);
    for (uint32_t i = 0; i < (max_name - name_len + 1); i++) nu_info(" ");
    nu_info("| %s", id);
    for (uint32_t i = 0; i < (max_id - id_len + 1); i++) nu_info(" ");
    nu_info("| %s", flag);
    for (uint32_t i = 0; i < (max_flag - flag_len + 1); i++) nu_info(" ");
    nu_info("| %s", interface);
    for (uint32_t i = 0; i < (max_interface - interface_len + 1); i++) nu_info(" ");
    nu_info("| %s", plugin);
    for (uint32_t i = 0; i < (max_plugin - plugin_len + 1); i++) nu_info(" ");
    nu_info("|\n");
}
nu_result_t nu_module_log(void)
{
    /* get module data */
    const nu_module_data_t *modules = (const nu_module_data_t*)nu_indexed_array_get_data(_system.modules);
    uint32_t module_count = nu_indexed_array_get_size(_system.modules);

    /* compute max */
    uint32_t max_name, max_id, max_flag, max_interface, max_plugin;
    compute_max(modules, module_count, &max_name, &max_id, &max_flag, &max_interface, &max_plugin);

    /* display header */
    log_transition_line(max_name, max_id, max_flag, max_interface, max_plugin);
    log_line(max_name, max_id, max_flag, max_interface, max_plugin, HEADER_NAME, HEADER_ID, HEADER_FLAGS, HEADER_INTERFACES, HEADER_PLUGINS);

    /* display */
    log_transition_line(max_name, max_id, max_flag, max_interface, max_plugin);
    for (uint32_t i = 0; i < module_count; i++) {
        const nu_module_data_t *module = &modules[i];
        char flags[MAX_MODULE_FLAG_COUNT][128];
        uint32_t flag_count, count;
        module_get_line_count_with_flags(module, flags, 128, &flag_count, &count);

        /* show line */
        for (uint32_t l = 0; l < count; l++) {
            char name[128];
            char id[128];
            char flag[128];
            char interface[128];
            char plugin[128];
            print_module_line_at_index(module, flags, flag_count, l, name, id, flag, interface, plugin, 128);
            log_line(max_name, max_id, max_flag, max_interface, max_plugin, name, id, flag, interface, plugin);
        }

        log_transition_line(max_name, max_id, max_flag, max_interface, max_plugin);
    }

    return NU_SUCCESS;
}