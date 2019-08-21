#include "pyro_pch.h"
#include "gl_texture.h"
#include "stb_image.h"
#include "glad/glad.h"

pyro::gl_texture_2d::gl_texture_2d(const std::string& path)
    : m_path(path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    PYRO_CORE_ASSERT(data, "[gl_texture_2d] Texture not loaded correctly");

    m_width = width;
    m_height = height;
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    // allocating memory to gpu to store the texture data
    const int mipmap_levels = 1;
		glTextureStorage2D(m_id, mipmap_levels, GL_RGB8, m_width, m_height);

    // set texture params 
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // upload texture to gpu 
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);

    // free the data
    stbi_image_free(data);
}

pyro::gl_texture_2d::~gl_texture_2d()
{
    glDeleteTextures(1, &m_id);
}

void pyro::gl_texture_2d::bind(uint32_t slot /*= 0*/) const
{
    glBindTextureUnit(slot, m_id);
}
