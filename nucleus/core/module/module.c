#include "module.h"

#define NU_LOGGER_MODULE_NAME "[MODULE] "

#define NU_MODULE_GET_INFO_NAME "nu_module_get_info"

#if defined(NU_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined(NU_PLATFORM_UNIX)
    #include <libgen.h>
#endif

#define MAX_MODULE_COUNT 64

typedef struct {
    nu_module_info_t info;
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
    char *path;

    /* reset memory */
    memset(module, 0, sizeof(nu_module_t));

    /* loading module */
#if defined(NU_PLATFORM_WINDOWS)
    char dir[256];
    char fname[128];
    _splitpath_s(filename, NULL, 0, dir, sizeof(dir), fname, sizeof(fname), NULL, 0);
    #if defined(__MINGW32__)
        path = nu_sprintf("%slib%s.dll", dir, fname);
    #else
        path = nu_sprintf("%s%s.dll", dir, fname);
    #endif
    module->handle = LoadLibraryA(path);
#elif defined(NU_PLATFORM_UNIX)
    char *dir, *fname;
    dir = dirname(filename);
    fname = basename(filename);
    path = nu_sprintf("%slib%s.so", dir, fname);
    module->handle = dlopen(path, RTLD_LAZY);
    free(dir);
    free(fname);
#endif
    nu_free(path);

    if (!module->handle) {
        nu_warning(NU_LOGGER_MODULE_NAME"Failed to load module '%s'.\n", filename);
        return NU_FAILURE;
    }

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
    *((uint64_t*)handle) = _data.module_count++;

    return NU_SUCCESS;
}
nu_result_t nu_module_load_function(nu_module_handle_t handle, const char *function_name, nu_pfn_t *function)
{
    return load_function(&_data.modules[(uint64_t)handle], function_name, function);
}
nu_id_t nu_module_get_id(nu_module_handle_t handle)
{
    return _data.modules[(uint64_t)handle].info.id;
}