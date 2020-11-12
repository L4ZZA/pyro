#include "pyro_pch.h"
#include "gl_frame_buffer_2d.h"
#include "glad/glad.h"
#include "gl_texture.h"
namespace pyro {
    static const uint32_t s_framebuffer_max_size = 8192;
}
pyro::gl_frame_buffer_2d::gl_frame_buffer_2d(framebuffer_props const &properties)
    : m_id(0)
    , m_depth_buffer_id(0)
    , m_width(properties.width)
    , m_height(properties.height)
    , m_clear_color(properties.clear_color)
{
    init();
}

pyro::gl_frame_buffer_2d::~gl_frame_buffer_2d()
{
    glDeleteFramebuffers(1, &m_id);
}

void pyro::gl_frame_buffer_2d::init()
{
    if(m_id)
    {
        glDeleteFramebuffers(1, &m_id);
        m_color_attachment.reset();
        //glDeleteTextures(1, &m_color_attachment);
        glDeleteTextures(1, &m_depth_attachment);
    }

    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    texture_parameters params;
    params.min_filter = e_texture_filter::linear;
    params.mag_filter = e_texture_filter::linear;
    params.format = e_texture_format::rgba;
    m_color_attachment = texture_2d::create(m_width, m_height, params);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment->id(), 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
    glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

    PYRO_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void pyro::gl_frame_buffer_2d::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, m_width, m_height);
}

void pyro::gl_frame_buffer_2d::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void pyro::gl_frame_buffer_2d::resize(uint32_t width, uint32_t height)
{

    if(width == 0 || height == 0 || width > s_framebuffer_max_size || height > s_framebuffer_max_size)
    {
        PYRO_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
        return;
    }
    m_width = width;
    m_height = height;
    init();
}

void pyro::gl_frame_buffer_2d::clear_color(glm::vec4 const &color)
{
    m_clear_color = color;
}

void pyro::gl_frame_buffer_2d::clear()
{
    glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

uint32_t pyro::gl_frame_buffer_2d::width() const
{
    return m_width;
}

uint32_t pyro::gl_frame_buffer_2d::height() const
{
    return m_height;
}

pyro::ref<pyro::texture_2d> const&
pyro::gl_frame_buffer_2d::color_attachment() const
{
    return m_color_attachment;
}

uint32_t
pyro::gl_frame_buffer_2d::depth_attachment() const
{
    return m_depth_buffer_id;
}
