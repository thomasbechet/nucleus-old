#include "renderer.hpp"

#include "common/logger.hpp"
#include "asset/shader.hpp"
#include "asset/mesh.hpp"
#include "asset/texture.hpp"
#include "asset/material.hpp"
#include "scene/camera.hpp"
#include "scene/staticmesh.hpp"

#include "../glfw/module/interface.h"

#include <GL/glew.h>

#include <fstream>
#include <streambuf>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

using namespace nugl;

struct content_t
{
    camera_t camera;
    
    uint32_t next_id = 0;
    uint32_t default_shader;
    std::unordered_map<uint32_t, std::unique_ptr<mesh_t>> meshes;
    std::unordered_map<uint32_t, std::unique_ptr<texture_t>> textures;
    std::unordered_map<uint32_t, std::unique_ptr<shader_t>> shaders;
    std::unordered_map<uint32_t, std::unique_ptr<staticmesh_t>> staticmeshes;
    std::unordered_map<uint32_t, std::pair<std::unique_ptr<material_t>, std::vector<uint32_t>>> materials;
};

typedef struct {
    nuglfw_window_interface_t glfw_interface;
    std::unique_ptr<content_t> content;
} nugl_data_t;

static nugl_data_t _data;

static nu_result_t load_glfw_interface(void)
{
    nu_module_handle_t window_module = nu_system_window_get_module_handle();
    if (nu_module_get_id(window_module) != NUGLFW_MODULE_ID) {
        fatal("Software rasterizer requires GLFW module to work.\n");
        return NU_FAILURE;
    }

    nuglfw_window_interface_loader_pfn_t load_interface;
    if (nu_module_load_function(window_module, NUGLFW_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
        log_warning("Software rasterizer failed to load glfw loader.\n");
        return NU_FAILURE;
    }

    if (load_interface(&_data.glfw_interface) != NU_SUCCESS) {
        log_warning("Software rasterizer failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

nu_result_t nugl::initialize(void)
{
    /* loading glfw interface */
    log_info("Loading glfw interface...\n");
    if (load_glfw_interface() != NU_SUCCESS) {
        log_warning("Failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    if (glewInit()) {
        log_warning("Failed to initialize glew.\n");
        return NU_FAILURE;
    }

    _data.content = std::make_unique<content_t>();

    /* default shader */
    std::ifstream vstream("engine/shader/opengl/shader.vert");
    std::string vertex_code((std::istreambuf_iterator<char>(vstream)),
        std::istreambuf_iterator<char>());

    std::ifstream fstream("engine/shader/opengl/shader.frag");
    std::string fragment_code((std::istreambuf_iterator<char>(fstream)),
        std::istreambuf_iterator<char>());

    uint32_t id = _data.content->next_id++;
    _data.content->default_shader = id;
    _data.content->shaders.emplace(id, std::make_unique<shader_t>());
    _data.content->shaders.at(id)->load_shader(vertex_code, fragment_code);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    return NU_SUCCESS;
}
nu_result_t nugl::terminate(void)
{
    _data.content.reset();

    return NU_SUCCESS;
}
nu_result_t nugl::render(void)
{
    /* get window size */
    uint32_t width, height;
    nu_window_get_size(&width, &height);

    /* get camera matrix */
    nu_mat4_t view, projection, vp;
    _data.content->camera.set_aspect((float)width / (float)height);
    _data.content->camera.get_view_matrix(view);
    _data.content->camera.get_projection_matrix(projection);
    nu_mat4_mul(projection, view, vp);

    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto &shader = *_data.content->shaders.at(_data.content->default_shader).get();
    shader.use();

    for (auto &it_material : _data.content->materials) {
        auto &material = *it_material.second.first.get();
        
        /* bind material */
        material_t::bind(material, _data.content->textures);

        for (auto &it_staticmesh : it_material.second.second) {
            auto &staticmesh = *_data.content->staticmeshes.at(it_staticmesh).get();
            auto &mesh = *_data.content->meshes.at(staticmesh.get_mesh()).get();

            /* mvp */
            nu_mat4_t mvp, transform;
            staticmesh.get_transform(transform);
            nu_mat4_mul(vp, transform, mvp);

            /* normal to view */
            nu_mat3_t normal_to_view, view3;
            nu_mat4_pick_mat3(transform, normal_to_view);
            nu_mat3_inverse(normal_to_view);
            nu_mat3_transpose(normal_to_view);
            nu_mat4_pick_mat3(view, view3);
            nu_mat3_mul(normal_to_view, view3, normal_to_view);

            /* push */
            glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "mvp"), 1, GL_FALSE, (float*)mvp);
            glUniformMatrix3fv(glGetUniformLocation(shader.get_program(), "normal_to_view"), 1, GL_FALSE, (float*)mvp);

            /* draw */
            mesh.draw_buffer();
        }
    }

    _data.glfw_interface.swap_buffers();

    return NU_SUCCESS;
}

nu_result_t nugl::mesh_create(nu_renderer_mesh_handle_t *handle, const nu_renderer_mesh_create_info_t *info)
{
    uint32_t id = _data.content->next_id++;
    _data.content->meshes.emplace(id, std::make_unique<mesh_t>(*info));
    NU_HANDLE_SET_ID(*handle, id);

    return NU_SUCCESS;
}
nu_result_t nugl::mesh_destroy(nu_renderer_mesh_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nugl::texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info)
{
    uint32_t id = _data.content->next_id++;
    _data.content->textures.emplace(id, std::make_unique<texture_t>(*info));
    NU_HANDLE_SET_ID(*handle, id);

    return NU_SUCCESS;
}
nu_result_t nugl::texture_destroy(nu_renderer_texture_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nugl::material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info)
{
    uint32_t id = _data.content->next_id++;
    _data.content->materials.emplace(id, 
        std::make_pair<std::unique_ptr<material_t>, std::vector<uint32_t>>(
        std::make_unique<material_t>(*info), std::vector<uint32_t>()));
    NU_HANDLE_SET_ID(*handle, id);

    return NU_SUCCESS;
}
nu_result_t nugl::material_destroy(nu_renderer_material_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nugl::font_create(nu_renderer_font_handle_t *handle, const nu_renderer_font_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nugl::font_destroy(nu_renderer_font_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nugl::font_get_text_size(nu_renderer_font_handle_t handle, const char *text, uint32_t *width, uint32_t *height)
{
    return NU_SUCCESS;
}

nu_result_t nugl::camera_create(nu_renderer_camera_handle_t *handle, const nu_renderer_camera_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nugl::camera_destroy(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nugl::camera_set_fov(nu_renderer_camera_handle_t handle, float fov)
{
    _data.content->camera.set_fov(fov);
    return NU_SUCCESS;
}
nu_result_t nugl::camera_set_eye(nu_renderer_camera_handle_t handle, const nu_vec3_t eye)
{
    _data.content->camera.set_eye(eye);
    return NU_SUCCESS;
}
nu_result_t nugl::camera_set_center(nu_renderer_camera_handle_t handle, const nu_vec3_t center)
{
    _data.content->camera.set_center(center);
    return NU_SUCCESS;
}
nu_result_t nugl::camera_set_active(nu_renderer_camera_handle_t handle)
{
    return NU_SUCCESS;
}

nu_result_t nugl::staticmesh_create(nu_renderer_staticmesh_handle_t *handle, const nu_renderer_staticmesh_create_info_t *info)
{
    uint32_t id = _data.content->next_id++;
    _data.content->staticmeshes.emplace(id, std::make_unique<staticmesh_t>(*info, id));
    NU_HANDLE_SET_ID(*handle, id);

    /* add to materials */
    auto &staticmesh = *_data.content->staticmeshes.at(id).get();
    _data.content->materials.at(staticmesh.get_material()).second.emplace_back(id);

    return NU_SUCCESS;
}
nu_result_t nugl::staticmesh_destroy(nu_renderer_staticmesh_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nugl::staticmesh_set_transform(nu_renderer_staticmesh_handle_t handle, const nu_mat4_t transform)
{
    return NU_SUCCESS;
}

nu_result_t nugl::label_create(nu_renderer_label_handle_t *handle, const nu_renderer_label_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nugl::label_destroy(nu_renderer_label_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nugl::label_set_position(nu_renderer_label_handle_t handle, int32_t x, int32_t y)
{
    return NU_SUCCESS;
}
nu_result_t nugl::label_set_text(nu_renderer_label_handle_t handle, const char *text)
{
    return NU_SUCCESS;
}

nu_result_t nugl::rectangle_create(nu_renderer_rectangle_handle_t *handle, const nu_renderer_rectangle_create_info_t *info)
{
    return NU_SUCCESS;
}
nu_result_t nugl::rectangle_destroy(nu_renderer_rectangle_handle_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nugl::rectangle_set_rect(nu_renderer_rectangle_handle_t handle, nu_rect_t rect)
{
    return NU_SUCCESS;
}

nu_result_t nugl::viewport_set_size(uint32_t width, uint32_t height)
{
    return NU_SUCCESS;
}
nu_result_t nugl::viewport_get_size(uint32_t *width, uint32_t *height)
{
    return NU_SUCCESS;
}