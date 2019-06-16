#include "pyro_pch.h"
#include "gl_buffer.h"
#include <glad\glad.h>

//=================== vertex buffer =======================================

pyro::gl_vertex_buffer::gl_vertex_buffer(float* vertices, uint32_t size)
{
    glGenBuffers(1, &m_id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

pyro::gl_vertex_buffer::~gl_vertex_buffer()
{
    glDeleteBuffers(1, &m_id);
}

void pyro::gl_vertex_buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void pyro::gl_vertex_buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void pyro::gl_vertex_buffer::layout(const buffer_layout& layout)
{
    m_layout = layout;
}

const pyro::buffer_layout& pyro::gl_vertex_buffer::layout() const
{
    return m_layout;
}

//=================== index buffer ========================================

pyro::gl_index_buffer::gl_index_buffer(uint32_t* vertices, uint32_t count)
    : m_count(count)
{
    glGenBuffers(1, &m_id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), vertices, GL_STATIC_DRAW);
}

pyro::gl_index_buffer::~gl_index_buffer()
{
    glDeleteBuffers(1, &m_id);
}

void pyro::gl_index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void pyro::gl_index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t pyro::gl_index_buffer::count() const
{
    return m_count;
}
