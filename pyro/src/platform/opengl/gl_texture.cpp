#include "pyro_pch.h"
#include "gl_texture.h"

#include "stb_image.h"
#include "glad/glad.h"

pyro::gl_texture_2d::gl_texture_2d(uint32_t width, uint32_t height)
    :m_width(width), m_height(height)
{
    m_internal_format = GL_RGBA8;
    m_data_format = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    // allocating memory to gpu to store the texture data
    const int mipmap_levels = 1;
    glTextureStorage2D(m_id, mipmap_levels, m_internal_format, m_width, m_height);

    // set texture params
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

pyro::gl_texture_2d::gl_texture_2d(std::string const &path)
    : m_path(path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    PYRO_CORE_ASSERT(data, "[gl_texture_2d] Texture not loaded correctly");
    m_width = width;
    m_height = height;

    GLenum internal_format = 0, data_format = 0;
    if(channels == 3)
    {
        internal_format = GL_RGB8;
        data_format = GL_RGB;
    }
    else if(channels == 4)
    {
        internal_format = GL_RGBA8;
        data_format = GL_RGBA;
    }

    m_internal_format = internal_format;
    m_data_format = data_format;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    // allocating memory to gpu to store the texture data
    const int mipmap_levels = 1;
    glTextureStorage2D(m_id, mipmap_levels, internal_format, m_width, m_height);

    // set texture params 
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // upload texture to gpu 
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, data_format, GL_UNSIGNED_BYTE, data);

    // freeing allocated image buffer
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

void pyro::gl_texture_2d::data(void *data, uint32_t size)
{
    uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
    PYRO_CORE_ASSERT(size == m_width * m_height * bpp, "Data must be the entire texture!");
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
}

