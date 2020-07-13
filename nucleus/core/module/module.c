#include "module.h"

#define NU_LOGGER_MODULE_NAME "[MODULE] "

#define NU_MODULE_GET_INFO_NAME "nu_module_get_info"

#ifdef NU_PLATFORM_WINDOWS
    #include <windows.h>
#endif

nu_result_t nu_module_load(nu_module_t *module, const char *module_name)
{
    char *path;

    /* reset memory */
    memset(module, 0, sizeof(nu_module_t));

    /* loading module */
#if defined(NU_PLATFORM_WINDOWS)
    #if defined(__MINGW32__)
        path = nu_sprintf("lib%s.dll", module_name);
    #else
        path = nu_sprintf("%s.dll", module_name);
    #endif
    module->handle = LoadLibraryA(path);
#elif defined(NU_PLATFORM_UNIX)
    path = nu_sprintf("lib%s.so", module_name);
    module->handle = dlopen(path, RTLD_LAZY);
#endif
    nu_free(path);

    if (!module->handle) {
        nu_warning(NU_LOGGER_MODULE_NAME"Failed to load module '%s'.\n", module_name);
        return NU_FAILURE;
    }

    /* load module info */
    nu_result_t (*module_get_info)(nu_module_info_t*);
    if (nu_module_load_function(module, NU_MODULE_GET_INFO_NAME, (nu_pfn_t*)&module_get_info) != NU_SUCCESS) {
        nu_warning(NU_LOGGER_MODULE_NAME"'%s' function is required to load the module '%s'.\n", NU_MODULE_GET_INFO_NAME, module_name);
        nu_module_unload(module);
        return NU_FAILURE;
    }

    if (module_get_info(&module->info) != NU_SUCCESS) {
        nu_warning(NU_LOGGER_MODULE_NAME"Failed to retrieve info from module '%s'.\n", module_name);
        nu_module_unload(module);
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nu_module_unload(const nu_module_t *module)
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
nu_result_t nu_module_load_function(const nu_module_t *module, const char *function_name, nu_pfn_t *function)
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