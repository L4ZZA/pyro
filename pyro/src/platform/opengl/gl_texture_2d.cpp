#include "pyro_pch.h"
#include "platform/opengl/gl_texture_2d.h"

#include "stb_image.h"
#include "glad/glad.h"

pyro::gl_texture_2d::gl_texture_2d(
    uint32_t width,
    uint32_t height,
    texture_parameters const &params)
    : m_path("no_path")
    , m_width(width)
    , m_height(height)
    , m_parameters(params)
{
	PYRO_PROFILE_FUNCTION();
    m_internal_format = GL_RGBA8;
    m_data_format = GL_RGBA;
    
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id); 

    // allocating memory to gpu to store the texture data
    const int mipmap_levels = 1;

    //glBindTexture(GL_TEXTURE_2D, m_id);
    //glTexStorage2D(GL_TEXTURE_2D, mipmap_levels, m_internal_format, m_width, m_height);
    //----------------
    // From OpenGL 4.5, The following line replaces the two above
    glTextureStorage2D(m_id, mipmap_levels, m_internal_format, m_width, m_height);

    // set texture params
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, texture_wrap_to_gl(s_wrap_mode));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, texture_wrap_to_gl(s_wrap_mode));
}

pyro::gl_texture_2d::gl_texture_2d(
    std::string const &path,
    texture_parameters const &params)
    : m_path(path)
    , m_parameters(params)
{
	PYRO_PROFILE_FUNCTION();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = nullptr;
    {
	    PYRO_PROFILE_SCOPE("stbi_load - gl_texture_2d::gl_texture_2d(std::string const &");
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }
    PYRO_CORE_ASSERT(data, "[gl_texture_2d] Texture not loaded correctly");
    m_width = width;
    m_height = height;

    GLenum internal_format = 0, data_format = 0;
    if(channels == 3)
    {
        m_parameters.format = e_texture_format::rgb;
        internal_format = GL_RGB8;
        data_format = GL_RGB;
    }
    else if(channels == 4)
    {
        m_parameters.format = e_texture_format::rgba;
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

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, texture_wrap_to_gl(s_wrap_mode));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, texture_wrap_to_gl(s_wrap_mode));

    // upload texture to gpu 
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, data_format, GL_UNSIGNED_BYTE, data);

    // freeing allocated image buffer
    stbi_image_free(data);
}

pyro::gl_texture_2d::~gl_texture_2d()
{
	PYRO_PROFILE_FUNCTION();
    glDeleteTextures(1, &m_id);
}

void pyro::gl_texture_2d::bind(uint32_t slot /*= 0*/) const
{
	PYRO_PROFILE_FUNCTION();
    glBindTextureUnit(slot, m_id);
}

void pyro::gl_texture_2d::data(void *data, uint32_t size)
{
	PYRO_PROFILE_FUNCTION();
    uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
    PYRO_CORE_ASSERT(size == m_width * m_height * bpp, "Data must be the entire texture!");
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
}


uint32_t pyro::gl_texture_2d::texture_wrap_to_gl(e_texture_wrap mode)
{
    switch(mode)
    {
        case e_texture_wrap::clamp_to_border:   return GL_CLAMP_TO_BORDER;
        case e_texture_wrap::clamp_to_edge:     return GL_CLAMP_TO_EDGE;
        case e_texture_wrap::repeat:            return GL_REPEAT;
        case e_texture_wrap::mirrored_repeat:   return GL_MIRRORED_REPEAT;
    }                                                  
    return 0;
}
