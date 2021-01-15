#include "shader.hpp"

#include "../common/logger.hpp"

using namespace nugl;

shader_t::~shader_t()
{
    unload();
}

bool shader_t::load_shader(const std::string &vertex_code, const std::string &fragment_code)
{
    GLint vertex_id, fragment_id;
    GLint success;
    char info_log[512];

    const char *vertexAddr = vertex_code.c_str();
    const char *fragmentAddr = fragment_code.c_str();

    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertexAddr, nullptr);
    glCompileShader(vertex_id);

    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_id, 512, nullptr, info_log);
        log_warning(std::string(info_log));
        return false;
    }

    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragmentAddr, nullptr);
    glCompileShader(fragment_id);

    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_id, 512, nullptr, info_log);
        log_warning(std::string(info_log));
        return false;
    }

    m_handle = glCreateProgram();
    glAttachShader(m_handle, vertex_id);
    glAttachShader(m_handle, fragment_id);
    glLinkProgram(m_handle);
    glDetachShader(m_handle, vertex_id);
    glDetachShader(m_handle, fragment_id);

    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_handle, 512, nullptr, info_log);
        log_warning(std::string(info_log));
        return false;
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    m_is_loaded = true;

    return true;
}
void shader_t::unload()
{
    if (is_loaded()) {
        glDeleteProgram(m_handle);
    }
    m_is_loaded = false;
}

bool shader_t::is_loaded() const
{
    return m_is_loaded;
}
GLuint shader_t::get_program() const
{
    return m_handle;
}
void shader_t::use() const
{
    glUseProgram(m_handle);
}