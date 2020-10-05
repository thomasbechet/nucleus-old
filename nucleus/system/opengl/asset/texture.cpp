#include "texture.hpp"

using namespace nugl;

texture_t::texture_t(const nu_renderer_texture_create_info_t &info)
{
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (info.channel == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.data);
    } else if (info.channel == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, info.data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}
texture_t::~texture_t()
{
    glDeleteTextures(1, &m_handle);
}

void texture_t::bind(uint32_t binding)
{
    glActiveTexture(GL_TEXTURE0 + binding);
    glBindTexture(GL_TEXTURE_2D, m_handle);
}