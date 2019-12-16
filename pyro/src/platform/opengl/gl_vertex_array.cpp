#include "pyro_pch.h"
#include "gl_vertex_array.h"
#include "glad/glad.h"

namespace pyro
{
    static uint32_t to_opengl_type(e_shader_data_type type)
    {
        switch (type)
        {
            case pyro::e_shader_data_type::float1:    return GL_FLOAT;
            case pyro::e_shader_data_type::float2:    return GL_FLOAT;
            case pyro::e_shader_data_type::float3:    return GL_FLOAT;
            case pyro::e_shader_data_type::float4:    return GL_FLOAT;
            case pyro::e_shader_data_type::int1:      return GL_INT;
            case pyro::e_shader_data_type::int2:      return GL_INT;
            case pyro::e_shader_data_type::int3:      return GL_INT;
            case pyro::e_shader_data_type::int4:      return GL_INT;
            case pyro::e_shader_data_type::mat3:      return GL_FLOAT;
            case pyro::e_shader_data_type::mat4:      return GL_FLOAT;
            case pyro::e_shader_data_type::boolean:   return GL_BOOL;
        }

        PYRO_CORE_ASSERT(false, "[to_opengl] Unknown shader_data_type!");
        return 0;
    }
}


pyro::gl_vertex_array::gl_vertex_array()
{
	PYRO_PROFILE_FUNCTION();
    glCreateVertexArrays(1, &m_id);
}

pyro::gl_vertex_array::~gl_vertex_array()
{
	PYRO_PROFILE_FUNCTION();
    glDeleteVertexArrays(1, &m_id);
}

void pyro::gl_vertex_array::bind() const
{
	PYRO_PROFILE_FUNCTION();
    glBindVertexArray(m_id);
}

void pyro::gl_vertex_array::unbind() const
{
	PYRO_PROFILE_FUNCTION();
    glBindVertexArray(0);
}

void pyro::gl_vertex_array::add_buffer(ref<vertex_buffer> const &vertex_buffer)
{
	PYRO_PROFILE_FUNCTION();
    glBindVertexArray(m_id);
    vertex_buffer->bind();
    const auto& layout = vertex_buffer->layout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(m_vertex_buffer_index);
        glVertexAttribPointer(m_vertex_buffer_index, 
            element.components_count(), 
            to_opengl_type(element.type), 
            element.normalised ? GL_TRUE : GL_FALSE, 
            layout.stride(), 
            reinterpret_cast<const void*>(element.offset)
        );
        m_vertex_buffer_index++;
    }
    m_vertex_buffers.push_back(vertex_buffer);
}

void pyro::gl_vertex_array::add_buffer(ref<pyro::index_buffer> const &index_buffer)
{
	PYRO_PROFILE_FUNCTION();
    glBindVertexArray(m_id);
    index_buffer->bind();
    m_index_buffer = index_buffer;
}

