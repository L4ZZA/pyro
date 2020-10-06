#include "pyro_pch.h"
#include "platform/opengl/gl_texture.h"

#include "stb_image.h"
#include "glad/glad.h"

pyro::gl_texture_2d::gl_texture_2d(
    uint32_t width,
    uint32_t height,
    texture_parameters const &params)
    : m_path("no_path")
    , m_width(width)
    , m_height(height)
    , m_format(params.format)
    , m_min_filter(params.min_filter)
    , m_mag_filter(params.mag_filter)
    , m_wrap(params.wrap)
{
	PYRO_PROFILE_FUNCTION();

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id); 

    //glBindTexture(GL_TEXTURE_2D, m_id);
    //glTexStorage2D(GL_TEXTURE_2D, mipmap_levels, m_internal_format, m_width, m_height);
    //----------------
    // From OpenGL 4.5, The following line replaces the two above
    // allocating memory to gpu to store the texture
    glTextureStorage2D(m_id, 1, 
        texture_internal_format_to_gl(m_format), m_width, m_height);

    // set texture params
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, 
        texture_filter_to_gl(m_min_filter));
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, 
        texture_filter_to_gl(m_mag_filter));

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, texture_wrap_to_gl(m_wrap));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, texture_wrap_to_gl(m_wrap));

    PYRO_CORE_TRACE("Created texture from data - id: {}", m_id);
}

pyro::gl_texture_2d::gl_texture_2d(
    std::string const &path,
    texture_parameters const &params)
    : m_path(path)
    , m_format(params.format)
    , m_min_filter(params.min_filter)
    , m_mag_filter(params.mag_filter)
    , m_wrap(params.wrap)
{
	PYRO_PROFILE_FUNCTION();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = nullptr;
    {
	    PYRO_PROFILE_SCOPE(
            "stbi_load - gl_texture_2d::gl_texture_2d(std::string const &");
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }
    PYRO_CORE_ASSERT(data, "[gl_texture_2d] Texture not loaded correctly");
    m_width = width;
    m_height = height;

    if(channels == 3)
    {
        m_format = e_texture_format::rgb;
    }
    else if(channels == 4)
    {
        m_format = e_texture_format::rgba;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    // allocating memory to gpu to store the texture data
    glTextureStorage2D(m_id, 1, 
        texture_internal_format_to_gl(m_format), m_width, m_height);

    // set texture params 
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, 
        texture_filter_to_gl(m_min_filter));
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, 
        texture_filter_to_gl(m_mag_filter));

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, texture_wrap_to_gl(m_wrap));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, texture_wrap_to_gl(m_wrap));

    // upload texture to gpu 
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, 
        texture_format_to_gl(m_format), GL_UNSIGNED_BYTE, data);

    // freeing allocated image buffer
    stbi_image_free(data);
    PYRO_CORE_TRACE(
        "[gl_texture_2d] Created texture from file - id: {}, path: {}", 
        m_id, m_path);
}

pyro::gl_texture_2d::~gl_texture_2d()
{
	PYRO_PROFILE_FUNCTION();
    //PYRO_CORE_TRACE("[~gl_texture_2d] Deleted texture - id: {}", m_id);
    glDeleteTextures(1, &m_id);
}

void pyro::gl_texture_2d::bind(uint32_t slot /*= 0*/) const
{
	PYRO_PROFILE_FUNCTION();
    glBindTextureUnit(slot, m_id);
}

void pyro::gl_texture_2d::data(
    void *data, 
    uint32_t size, 
    e_texture_data_type type /*= e_texture_data_type::unsigned_byte*/)
{
	PYRO_PROFILE_FUNCTION();
    uint32_t bpp = bytes_per_pixel();
    int32_t expected_size = m_width * m_height * bpp;
    PYRO_CORE_ASSERT(size == expected_size, "Data must be the entire texture!");
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, 
        texture_format_to_gl(m_format), texture_data_type_to_gl(type), data);
}

uint32_t pyro::gl_texture_2d::bytes_per_pixel() const
{
    uint32_t bpp = 0;
    switch (m_format)
    {
    case pyro::e_texture_format::red:
        bpp = 1;
        break;
    case pyro::e_texture_format::rg:
        bpp = 2;
        break;
    case pyro::e_texture_format::rgb:
        bpp = 3;
        break;
    case pyro::e_texture_format::rgba:
        bpp = 4;
        break;
    }
    return bpp;
}

uint32_t pyro::gl_texture_2d::texture_format_to_gl(e_texture_format format)
{
    // see explanation of luminance [opengl4.5+]
    // https://www.gamedev.net/forums/topic/634850-do-luminance-texture39s-still-exist-to-opengl/5003839/
    switch(format)
    {
        case e_texture_format::rgba: return GL_RGBA;// 4 channels
        case e_texture_format::rgb:	 return GL_RGB; // 3 channels
        case e_texture_format::rg:	 return GL_RG;  // 2 channels 
        case e_texture_format::red:	 return GL_RED; // single channel
    }
    return 0;
}

uint32_t pyro::gl_texture_2d::texture_data_type_to_gl(e_texture_data_type type)
{
    switch (type)
    {
    case e_texture_data_type::unsigned_byte:	return GL_UNSIGNED_BYTE; 
    case e_texture_data_type::Float:	        return GL_FLOAT;
    }
    return GL_UNSIGNED_BYTE;
}

uint32_t pyro::gl_texture_2d::texture_internal_format_to_gl(e_texture_format format)
{
    switch(format)
    {
        case e_texture_format::rgba:    return GL_RGBA8;// 4 channels
        case e_texture_format::rgb:	    return GL_RGB8; // 3 channels
        case e_texture_format::rg:	    return GL_RG8;  // 2 channels 
        case e_texture_format::red:	    return GL_R8; // single channel
    }
    return 0;
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

uint32_t pyro::gl_texture_2d::texture_filter_to_gl(e_texture_filter mode)
{
    switch(mode)
    {
        case e_texture_filter::linear:  return GL_LINEAR;
        case e_texture_filter::nearest: return GL_NEAREST;
    }                                                  
    return 0;
}
