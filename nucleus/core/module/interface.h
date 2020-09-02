#ifndef NU_MODULE_INTERFACE_H
#define NU_MODULE_INTERFACE_H

#include "../common/common.h"

/* plugin interface */
#define NU_PLUGIN_INTERFACE_NAME        "nu_plugin_interface"
#define NU_PLUGIN_INTERFACE_LOADER_NAME "nu_plugin_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*late_update)(void);
    nu_result_t (*fixed_update)(void);
} nu_plugin_interface_t;

typedef nu_result_t (*nu_plugin_interface_loader_pfn_t)(nu_plugin_interface_t*, const char*);

/* task system interface */
#define NU_TASK_INTERFACE_NAME        "nu_task_interface"
#define NU_TASK_INTERFACE_LOADER_NAME "nu_task_get_interface"

NU_DECLARE_HANDLE(nu_task_handle_t);

typedef struct {
    void (*func)(void*, uint32_t, uint32_t);
    void *args;
} nu_task_job_t;

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*create)(nu_task_handle_t *task);
    nu_result_t (*perform)(nu_task_handle_t task, nu_task_job_t *jobs, uint32_t count);
    nu_result_t (*wait)(nu_task_handle_t task);
    bool (*is_completed)(nu_task_handle_t task);
} nu_task_interface_t;

typedef nu_result_t (*nu_task_interface_loader_pfn_t)(nu_task_interface_t*);

/* window system interface */
#define NU_WINDOW_INTERFACE_NAME        "nu_window_interface"
#define NU_WINDOW_INTERFACE_LOADER_NAME "nu_window_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_size)(uint32_t*, uint32_t*);
} nu_window_interface_t;

typedef nu_result_t (*nu_window_interface_loader_pfn_t)(nu_window_interface_t*);

/* input system interface */
#define NU_INPUT_INTERFACE_NAME        "nu_input_interface"
#define NU_INPUT_INTERFACE_LOADER_NAME "nu_input_get_interface"

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*update)(void);
    nu_result_t (*get_keyboard_state)(nu_button_state_t*, nu_keyboard_t);
    nu_result_t (*get_keyboard_text)(const char **text, uint32_t *length);
    nu_result_t (*get_mouse_state)(nu_button_state_t*, nu_mouse_t);
    nu_result_t (*get_mouse_motion)(nu_vec2_t);
    nu_result_t (*get_cursor_mode)(nu_cursor_mode_t*);
    nu_result_t (*set_cursor_mode)(nu_cursor_mode_t);
} nu_input_interface_t;

typedef nu_result_t (*nu_input_interface_loader_pfn_t)(nu_input_interface_t*);

/* renderer system interface */
#define NU_RENDERER_INTERFACE_NAME        "nu_renderer_interface"
#define NU_RENDERER_INTERFACE_LOADER_NAME "nu_renderer_get_interface"

typedef struct {
    uint32_t vertice_count;
    bool use_indices;
    bool use_colors;
    nu_vec3_t *positions;
    nu_vec2_t *uvs;
    nu_vec3_t *colors;
    uint32_t *position_indices;
    uint32_t *uv_indices;
    uint32_t *color_indices;
} nu_renderer_mesh_create_info_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t channel;
    unsigned char *data;
} nu_renderer_texture_create_info_t;

typedef struct {
    const char *filename;
} nu_renderer_font_create_info_t;

typedef struct {
    nu_vec3_t eye;
    nu_vec3_t center;
    nu_vec3_t up;
    float fov;
} nu_renderer_camera_create_info_t;

typedef struct {
    nu_renderer_mesh_handle_t mesh;
    nu_renderer_texture_handle_t texture;
    nu_mat4_t transform;
} nu_renderer_staticmesh_create_info_t;

NU_DECLARE_HANDLE(nu_renderer_mesh_handle_t);
NU_DECLARE_HANDLE(nu_renderer_font_handle_t);
NU_DECLARE_HANDLE(nu_renderer_texture_handle_t);
NU_DECLARE_HANDLE(nu_renderer_camera_handle_t);
NU_DECLARE_HANDLE(nu_renderer_staticmesh_handle_t);

typedef struct {
    nu_result_t (*initialize)(void);
    nu_result_t (*terminate)(void);
    nu_result_t (*render)(void);

    nu_result_t (*mesh_create)(nu_renderer_mesh_handle_t*, const nu_renderer_mesh_create_info_t*);
    nu_result_t (*mesh_destroy)(nu_renderer_mesh_handle_t);

    nu_result_t (*texture_create)(nu_renderer_texture_handle_t*, const nu_renderer_texture_create_info_t*);
    nu_result_t (*texture_destroy)(nu_renderer_texture_handle_t);

    nu_result_t (*font_create)(nu_renderer_font_handle_t*, const nu_renderer_font_create_info_t*);
    nu_result_t (*font_destroy)(nu_renderer_font_handle_t);

    nu_result_t (*camera_create)(nu_renderer_camera_handle_t*, const nu_renderer_camera_create_info_t*);
    nu_result_t (*camera_destroy)(nu_renderer_camera_handle_t);
    nu_result_t (*camera_set_fov)(nu_renderer_camera_handle_t, float);
    nu_result_t (*camera_set_eye)(nu_renderer_camera_handle_t, const nu_vec3_t);
    nu_result_t (*camera_set_center)(nu_renderer_camera_handle_t, const nu_vec3_t);
    nu_result_t (*camera_set_active)(nu_renderer_camera_handle_t);

    nu_result_t (*staticmesh_create)(nu_renderer_staticmesh_handle_t*, const nu_renderer_staticmesh_create_info_t*);
    nu_result_t (*staticmesh_destroy)(nu_renderer_staticmesh_handle_t);
    nu_result_t (*staticmesh_set_transform)(nu_renderer_staticmesh_handle_t, const nu_mat4_t);
} nu_renderer_interface_t;

typedef nu_result_t (*nu_renderer_interface_loader_pfn_t)(nu_renderer_interface_t*);

#endif