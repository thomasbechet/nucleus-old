#pragma once

#include <GL/glew.h>

#include <string>

namespace nugl
{
    class shader_t
    {
    public:
        ~shader_t();

        bool load_shader(const std::string &vertex, const std::string &fragment);
        void unload();

        bool is_loaded() const;
        uint32_t get_program() const;
        void use() const;

    private:
        bool m_is_loaded = false;
        GLuint m_handle;
    };
}