#include "renderer.h"

#include "../module/interface.h"
#include "../context/config.h"
#include "../../vulkan/module/interface.h"

#define NU_RENDERER_LOG_NAME "[RENDERER] "

static const char *nu_renderer_api_names[] = {
    "nucleus-renderer-none",
    NUVK_MODULE_NAME,
    "nucleus-opengl"
};

typedef struct {
    nu_module_t module;
    nu_renderer_interface_t interface;
} nu_system_renderer_t;

static nu_system_renderer_t _system;

nu_result_t nu_system_renderer_load(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    memset(&_system, 0, sizeof(nu_system_renderer_t));
    nu_renderer_api_t api = nu_config_get().renderer_api;

    /* load renderer module */
    result = nu_module_load(&_system.module, nu_renderer_api_names[api]);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load renderer interface accessor */
    nu_renderer_interface_loader_pfn_t load_interface;
    result = nu_module_load_function(&_system.module, NU_RENDERER_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_RENDERER_LOG_NAME" Failed to load renderer loader.\n");
        return result;
    }

    /* load renderer interface */
    result = load_interface(&_system.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_RENDERER_LOG_NAME" Failed to load interface.\n");
        return result;
    }

    /* initialize renderer system */
    result = _system.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_RENDERER_LOG_NAME"Failed to initialize renderer system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_system_renderer_unload(void)
{
    /* terminate renderer system */
    _system.interface.terminate();

    /* unload module */
    nu_module_unload(&_system.module);
    memset(&_system.interface, 0, sizeof(nu_renderer_interface_t));

    return NU_SUCCESS;
}
nu_result_t nu_system_renderer_render(void)
{
    _system.interface.render();
    return NU_SUCCESS;
}

const nu_module_t *nu_system_renderer_get_module(void)
{
    return &_system.module;
}