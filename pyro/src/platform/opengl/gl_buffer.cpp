#include "pyro_pch.h"
#include "gl_buffer.h"
#include <glad\glad.h>

//=================== vertex buffer =======================================

pyro::gl_vertex_buffer::gl_vertex_buffer(uint32_t size)
{
    PYRO_PROFILE_FUNCTION();
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

pyro::gl_vertex_buffer::gl_vertex_buffer(const float* vertices, uint32_t size)
{
	PYRO_PROFILE_FUNCTION();
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

pyro::gl_vertex_buffer::~gl_vertex_buffer()
{
	PYRO_PROFILE_FUNCTION();
    glDeleteBuffers(1, &m_id);
}

void pyro::gl_vertex_buffer::bind() const
{
	PYRO_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void pyro::gl_vertex_buffer::unbind() const
{
	PYRO_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void pyro::gl_vertex_buffer::data(void const* data, uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void pyro::gl_vertex_buffer::layout(const buffer_layout& layout)
{
    m_layout = layout;
}

pyro::buffer_layout const& pyro::gl_vertex_buffer::layout() const
{
    return m_layout;
}

//=================== index buffer ========================================

pyro::gl_index_buffer::gl_index_buffer(const uint32_t* vertices, uint32_t count)
    : m_count(count)
{
	PYRO_PROFILE_FUNCTION();
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), vertices, GL_STATIC_DRAW);
}

pyro::gl_index_buffer::~gl_index_buffer()
{
	PYRO_PROFILE_FUNCTION();
    glDeleteBuffers(1, &m_id);
}

void pyro::gl_index_buffer::bind() const
{
	PYRO_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void pyro::gl_index_buffer::unbind() const
{
	PYRO_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t pyro::gl_index_buffer::count() const
{
    return m_count;
}
