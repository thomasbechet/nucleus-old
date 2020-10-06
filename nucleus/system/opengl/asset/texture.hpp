#pragma once

#include "../../../core/nucleus.h"

#include <GL/glew.h>

namespace nugl
{
    class texture_t
    {
    public:
        texture_t(const nu_renderer_texture_create_info_t &info);
        ~texture_t();

        void bind(uint32_t binding);

    private:
        GLuint m_handle;
    };
}