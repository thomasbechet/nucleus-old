#include <nucleus/core/system/module/module.h>

#include <nucleus/core/system/module/module.h>
#include <nucleus/core/system/module/api.h>
#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/utility/table_printer.h>

#if defined(NU_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined(NU_PLATFORM_UNIX)
    #include <libgen.h>
    #include <dlfcn.h>
#endif

#define NU_MODULE_INFO_FUNCTION_NAME "nu_module_info"

typedef struct {
    // API name
    nu_string_t name;
    // Pointer to the API function structure
    void *data;
    // Size of the API structure
    size_t size;
    // Attached module, NU_NULL_HANDLE if the API is not assigned
    nu_module_t module;
} nu_api_data_t;

typedef struct {
    // True if the module is static
    bool static_module;
    // Keep the module id
    nu_module_t handle;
    // Module info
    nu_module_info_t info;
    // Constant module path, NU_NULL_HANDLE when static
    nu_string_t path;
    // Module OS handle, NULL when static
    void *library;
    // Persistent module data on core allocator
    void *persitent_data;
    // Size of the persistent data
    size_t persitent_data_size;
} nu_module_data_t;

typedef struct {
    nu_vector(nu_module_data_t) modules;
    nu_vector(nu_api_data_t) apis;
    uint32_t next_id;
    nu_module_t core_module;
} nu_state_t;

static nu_state_t s_state;

static nu_module_api_t s_module_api = {
    .open        = nu_module_open,
    .open_static = nu_module_open_static,
    .close       = nu_module_close,
    .hotreload   = nu_module_hotreload,
    .log         = nu_module_log
};

// static bool module_equals_by_path(const void *a, const void *user)
// {
//     const nu_module_data_t *module = a;
//     const char *path               = user;
//     return NU_MATCH(module->path, path);
// }
static bool module_equals_by_handle(const void *a, const void *user)
{
    const nu_module_data_t *module = a;
    const nu_module_t *handle      = user;
    return module->handle == *handle;
}
static bool module_equals_by_name(const void *a, const void *user)
{
    const nu_module_data_t *module = a;
    const char *name               = user;
    return NU_MATCH(module->info.name, name);
}
static bool api_equals_by_name(const void *a, const void *user)
{
    const nu_api_data_t *api = a;
    const char *name         = user;
    return NU_MATCH(api->name, name);
}
typedef struct {
    const char *name;
    nu_module_t module;
} nu_api_name_module_key_t;
static bool api_equals_by_name_and_module(const void *a, const void *user)
{
    const nu_api_data_t *api            = (const nu_api_data_t*)a;
    const nu_api_name_module_key_t *key = (const nu_api_name_module_key_t*)user;
    return NU_MATCH(api->name, key->name) && api->module == key->module;
}
static bool api_equals_orphan(const void *a, const void *user)
{
    const nu_api_data_t *api = (const nu_api_data_t*)a;
    const char *name         = (const char*)user;       
    return NU_MATCH(api->name, name) && api->module == NU_NULL_HANDLE;
}

static nu_pfn_t get_library_function(const void *raw_handle, const char *function_name)
{
#if defined(NU_PLATFORM_WINDOWS)
    UINT old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    nu_pfn_t function = (nu_pfn_t)GetProcAddress((HMODULE)raw_handle, function_name);
    SetErrorMode(old_mode);
#elif defined(NU_PLATFORM_UNIX)
    nu_pfn_t function = dlsym((void*)raw_handle, function_name);
#endif
    return function;
}
static nu_string_t compute_final_path(const char *path_cstr)
{
    // Resolve path
    nu_string_t path = nu_string_allocate(nu_allocator_get_core(), path_cstr);
    nu_string_resolve_path(&path);

    // Extract directory and filename
    nu_string_t filename = nu_string_allocate_filename(nu_allocator_get_core(), path);
    nu_string_t directory = nu_string_allocate_directory(nu_allocator_get_core(), path);

    // Compute final path
#if defined(NU_PLATFORM_WINDOWS)
    #if defined(__MINGW32__)
        nu_string_t final_path = nu_string_allocate_format(nu_allocator_get_core(), 
            "%slib%s.dll", directory, filename);
    #else
        nu_string_t final_path = nu_string_allocate_format(nu_allocator_get_core(), "%s%s.dll", 
            directory, filename);
    #endif
#elif defined(NU_PLATFORM_UNIX)
    nu_string_t final_path = nu_string_allocate_format(nu_allocator_get_core(), 
        "%slib%s.so", directory, filename);
#endif

    // Free resources
    nu_string_free(path);
    nu_string_free(directory);
    nu_string_free(filename);

    return final_path;
}
static void *load_library(const char *path)
{
#if defined(NU_PLATFORM_WINDOWS)    
    return LoadLibraryA(path);
#elif defined(NU_PLATFORM_UNIX)
    return dlopen(path, RTLD_LAZY);
#endif
}
static void unload_library(void *raw_handle)
{
#if defined(NU_PLATFORM_WINDOWS)
    UINT old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    FreeLibrary((HMODULE)raw_handle);
    SetErrorMode(old_mode);
#elif defined(NU_PLATFORM_UNIX)
    dlclose(raw_handle);
#endif
}
static void invalidate_apis(nu_vector(nu_api_data_t) apis, nu_module_t handle)
{
    for (nu_api_data_t *it = apis; it != nu_vector_end(apis); it++) {
        if (it->module == handle) {
            memset(it->data, 0, it->size);
            it->module = NU_NULL_HANDLE;
        }
    }
}

// +--------------------------------------------------------------------------+
// |                              PRIVATE API                                 |
// +--------------------------------------------------------------------------+

nu_result_t nu_module_initialize(void)
{
    // Allocate resources
    s_state.modules = nu_vector_allocate(nu_allocator_get_core(), nu_module_data_t);
    s_state.apis    = nu_vector_allocate(nu_allocator_get_core(), nu_api_data_t);
    s_state.next_id = 0;

    // Create core module
    nu_module_info_t core_info = {
        .name             = "core",
        .callbacks.load   = NULL,
        .callbacks.unload = NULL
    };
    s_state.core_module = nu_module_open_static(core_info);

    // Register module API
    nu_module_register_api(s_state.core_module, nu_module_api_t, &s_module_api);

    return NU_SUCCESS;
}
nu_result_t nu_module_terminate(void)
{
    // Modules
    while (!nu_vector_empty(s_state.modules)) {
        nu_module_close(s_state.modules->handle);
    }
    nu_vector_free(s_state.modules);

    // APIs
    for (nu_api_data_t *it = s_state.apis; it != nu_vector_end(s_state.apis); it++) {
        nu_free(nu_allocator_get_core(), it->data);
        nu_string_free(it->name);
    }
    nu_vector_free(s_state.apis);

    return NU_SUCCESS;
}
nu_module_t nu_module_get_core(void)
{
    return s_state.core_module;
}

// +--------------------------------------------------------------------------+
// |                               PUBLIC API                                 |
// +--------------------------------------------------------------------------+

nu_module_t nu_module_open(const char *path)
{
    // Compute final path
    nu_string_t final_path = compute_final_path(path);

    // Initialize values
    nu_module_data_t module;
    memset(&module, 0, sizeof(module));
    module.static_module = false;
    module.path          = final_path;

    // Load the library
    module.library = load_library(final_path);
    NU_CHECK(module.library, goto cleanup0, nu_logger_get_core(), "Failed to load library '%s'.", final_path);

    // Get the info function pointer
    nu_module_info_pfn_t info_pfn = (nu_module_info_pfn_t)get_library_function(module.library, NU_MODULE_INFO_FUNCTION_NAME);
    NU_CHECK(info_pfn, goto cleanup1, nu_logger_get_core(), "Failed to get module info.");

    // Get info
    memset(&module.info, 0, sizeof(module.info));
    info_pfn(&module.info);

    // Ensure the name doesn't exists
    NU_CHECK(nu_vector_find(s_state.modules, module_equals_by_name, module.info.name) == NULL, goto cleanup1,
        nu_logger_get_core(), "Module with name '%s' already exists.", module.info.name);

    // Create the module
    NU_HANDLE_SET_ID(module.handle, s_state.next_id++);
    nu_vector_push_value(&s_state.modules, module);

    // Invoke load callback
    if (module.info.callbacks.load) {
        module.info.callbacks.load(module.handle, false);
    }

    return module.handle;

cleanup1:
    unload_library(module.library);
cleanup0:
    nu_string_free(final_path);

    return NU_NULL_HANDLE;
}
nu_module_t nu_module_open_static(nu_module_info_t info)
{
    // Initialize values
    nu_module_data_t module;
    memset(&module, 0, sizeof(module));
    module.static_module = true;
    module.path          = NU_NULL_HANDLE;

    // Save info
    module.info = info;

    // Ensure the name doesn't exists
    NU_CHECK(nu_vector_find(s_state.modules, module_equals_by_name, module.info.name) == NULL, return NU_FAILURE,
        nu_logger_get_core(), "Module with name '%s' already exists.", module.info.name);

    // Create the module
    NU_HANDLE_SET_ID(module.handle, s_state.next_id++); 
    nu_vector_push_value(&s_state.modules, module);

    // Invoke load callback
    if (module.info.callbacks.load) {
        module.info.callbacks.load(module.handle, false);
    }

    return module.handle;
}
nu_result_t nu_module_close(nu_module_t handle)
{
    // Find the module
    uint32_t module_index;
    bool found = nu_vector_find_index(s_state.modules, module_equals_by_handle, &handle, &module_index);
    NU_CHECK(found, return NU_FAILURE, nu_logger_get_core(), "Failed to find module.");
    nu_module_data_t *module = &s_state.modules[module_index];

    // Call unload callback
    if (module->info.callbacks.unload) {
        module->info.callbacks.unload(module->handle, false);
    }

    // Invalidate APIs
    invalidate_apis(s_state.apis, handle);

    // Free persistent state
    if (module->persitent_data) {
        nu_free(nu_allocator_get_core(), module->persitent_data);
    }

    // Free resources
    if (module->library) {
        unload_library(module->library);
    }
    if (module->path) {
        nu_string_free(module->path);
    }

    // Remove from module table
    nu_vector_erase(s_state.modules, module_index);

    return NU_SUCCESS;
}
nu_result_t nu_module_hotreload(nu_module_t handle)
{
    // Find module
    nu_module_data_t *module = nu_vector_find(s_state.modules, module_equals_by_handle, &handle);
    NU_CHECK(module != NULL, return NU_FAILURE, nu_logger_get_core(), "Failed to find module.");

    // Check is static
    if (module->static_module) return NU_SUCCESS;

    // Keep old info
    void *old_library = module->library;
    nu_module_info_t old_info = module->info;

    // Load new library
    module->library = load_library(module->path);
    NU_CHECK(module->library, goto cleanup0, nu_logger_get_core(), "Failed to reload library.");

    // Get the info function pointer
    nu_module_info_pfn_t info_pfn = (nu_module_info_pfn_t)get_library_function(module->library, NU_MODULE_INFO_FUNCTION_NAME);
    NU_CHECK(info_pfn, goto cleanup1, nu_logger_get_core(), "Failed to get module info.");

    // Get new module info
    memset(&module->info, 0, sizeof(module->info));
    info_pfn(&module->info);

    // Ensure the name has not changed
    NU_CHECK(NU_MATCH(module->info.name, old_info.name), goto cleanup2, nu_logger_get_core(),
        "Module name updated, unable to hotreload.");

    // Hotunload
    if (old_info.callbacks.unload) {
        old_info.callbacks.unload(module->handle, true);
    }

    // Unload old module
    unload_library(old_library);

    // Hotreload
    if (module->info.callbacks.load) {
        module->info.callbacks.load(module->handle, true);
    }

    return NU_SUCCESS;

cleanup2:
    module->info = old_info;
cleanup1:
    unload_library(module->library);
cleanup0:
    module->library = old_library;

    return NU_FAILURE;
}
nu_result_t nu_module_register_api_(
    nu_module_t handle,
    const char *name,
    size_t size,
    const void *data
)
{
    // Find module
    nu_module_data_t *module = nu_vector_find(s_state.modules, module_equals_by_handle, &handle);
    NU_CHECK(module != NULL, return NU_FAILURE, nu_logger_get_core(), "Failed to find module.");

    // Find existing API owned by the module
    nu_api_name_module_key_t key = {
        .name   = name,
        .module = handle
    };
    nu_api_data_t *api = nu_vector_find(s_state.apis, api_equals_by_name_and_module, &key);
    if (api != NULL) {

        // Check size
        NU_CHECK(api->size == size, return NU_FAILURE, nu_logger_get_core(),
            "Found existing API '%s' but size doesn't match.", name);

        // The API has already be registered by this module
        // Update the module API
        memcpy(api->data, data, size);

        return NU_SUCCESS;
    }

    // Find orphan API
    api = nu_vector_find(s_state.apis, api_equals_orphan, &name);
    if (api != NULL) {

        // Check size
        NU_CHECK(api->size == size, return NU_FAILURE, nu_logger_get_core(), 
            "Found existing orphan API '%s' but size doesn't match.", name);

        // Setup the orphan API with the module
        api->module = handle;
        memcpy(api->data, data, size);

        return NU_SUCCESS;
    }

    // Create new module API
    api = nu_vector_push(&s_state.apis);
    api->name   = nu_string_allocate(nu_allocator_get_core(), name);
    api->module = handle;
    api->size   = size;
    api->data   = nu_malloc(nu_allocator_get_core(), size);

    return NU_SUCCESS;
}
void *nu_module_get_persitent_state_(
    nu_module_t handle,
    size_t size
)
{
    // Find module
    nu_module_data_t *module = nu_vector_find(s_state.modules, module_equals_by_handle, &handle);
    NU_CHECK(module != NULL, return NULL, nu_logger_get_core(), "Failed to find module.");

    // Check existing state
    if (module->persitent_data) {
        // Check size
        if (size != module->persitent_data_size) {
            nu_warning(nu_logger_get_core(), "Register persistent data size with different size.");
            return NULL;
        }
    } else {
        // Create persistent state
        module->persitent_data      = nu_malloc(nu_allocator_get_core(), size);
        module->persitent_data_size = size;
        memset(module->persitent_data, 0, size);
    }

    return module->persitent_data;
}
void *nu_module_get_api_(nu_module_t module, const char *name, size_t size)
{
    // Find the api
    nu_api_name_module_key_t key = {
        .module = module,
        .name   = name
    };
    nu_api_data_t *api = nu_vector_find(s_state.apis, api_equals_by_name_and_module, &key);

    // Check if the API exists
    if (api != NULL) {
        
        // Check the size (simply warn the user)
        if (size != api->size) {
            nu_warning(nu_logger_get_core(), "API size named '%s' doesn't match in size.", name);
            return NULL;
        }

        // Return the API
        return api->data;

    } else {

        // Create orphan API
        nu_api_data_t *api = nu_vector_push(&s_state.apis);

        // Initialize orphan API
        api->module = module;
        api->name   = nu_string_allocate(nu_allocator_get_core(), name);
        api->size   = size;
        api->data   = nu_malloc(nu_allocator_get_core(), size);
        memset(api->data, 0, size);

        // Return empty memory block
        return api->data;
    }
}
void *nu_api_get_(const char *name, size_t size)
{
    // Find the api
    nu_api_data_t *api = nu_vector_find(s_state.apis, api_equals_by_name, name);

    // Check if the API exists
    if (api != NULL) {
        
        // Check the size (simply warn the user)
        if (size != api->size) {
            nu_warning(nu_logger_get_core(), "API size named '%s' doesn't match in size.", name);
            return NULL;
        }

        // Return the API
        return api->data;

    } else {

        // Create orphan API
        nu_api_data_t *api = nu_vector_push(&s_state.apis);

        // Initialize orphan API
        api->module = NU_NULL_HANDLE;
        api->name   = nu_string_allocate(nu_allocator_get_core(), name);
        api->size   = size;
        api->data   = nu_malloc(nu_allocator_get_core(), size);
        memset(api->data, 0, size);

        // Return empty memory block
        return api->data;
    }
}
void nu_module_log(void)
{
    nu_table_printer_t tp = nu_table_printer_allocate(nu_allocator_get_core());

    // Header
    nu_table_printer_row_center(tp, "%s", "MODULES");
    nu_table_printer_row_center(tp, "%s|%s|%s", "NAME", "TYPE", "API");
    
    // Log modules
    nu_table_printer_separator(tp);
    for (nu_module_data_t *data_it = s_state.modules; data_it != nu_vector_end(s_state.modules); data_it++) {
        
        // Line separator
        if (data_it != s_state.modules) {
            nu_table_printer_separator(tp);
        }

        // Find first api
        nu_api_data_t *first = NULL;
        for (nu_api_data_t *api_it = s_state.apis; api_it != nu_vector_end(s_state.apis); api_it++) {
            if (api_it->module == data_it->handle) {
                first = api_it;
                break;
            }
        }
        
        // Module info
        nu_table_printer_row(tp, "%s|%s|%s",
            data_it->info.name,
            data_it->static_module ? "static" : "shared",
            first != NULL ? first->name : "none");
        
        // APIs
        if (first) {
            for (nu_api_data_t *api_it = first + 1; api_it != nu_vector_end(s_state.apis); api_it++) {
                if (api_it->module == data_it->handle) {
                    nu_table_printer_row(tp, "%s|%s|%s", "", "", api_it->name);
                }
            }
        }
    }

    // Log other APIs
    nu_table_printer_row_center(tp, "%s", "ANONYMOUS API");
    nu_table_printer_separator(tp);
    uint32_t api_without_module_count = 0;
    for (nu_api_data_t *api_it = s_state.apis; api_it != nu_vector_end(s_state.apis); api_it++) {
        if (api_it->module == NU_NULL_HANDLE) {
            nu_table_printer_row(tp, "%s", api_it->name);
            api_without_module_count++;
        }
    }
    if (!api_without_module_count) {
        nu_table_printer_row(tp, "%s", "none");
    }
    
    // Render and free resources
    nu_info(nu_logger_get_core(), "Modules information :\n%s", nu_table_printer_render(tp));
    nu_table_printer_free(tp);
}