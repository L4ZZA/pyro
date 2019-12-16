#include "pyro_pch.h"
#include "buffer.h"
#include "renderer.h"
#include "platform/opengl/gl_buffer.h"

//=================== vertex buffer =======================================

pyro::ref<pyro::vertex_buffer> pyro::vertex_buffer::create(const float * vertices, uint32_t size)
{
    switch (renderer::api())
    {
    case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[vertex_buffer] e_renderer_api::none currently not supported!"); return nullptr;
    case renderer_api::e_api::open_gl: return make_ref<gl_vertex_buffer>(vertices, size);
    }

    PYRO_CORE_ASSERT(false, "[vertex_buffer] Unknown renderer api!");
    return nullptr;
}

//=================== index buffer ========================================

pyro::ref<pyro::index_buffer> pyro::index_buffer::create(const uint32_t* indices, uint32_t count)
{
    switch (renderer::api())
    {
    case renderer_api::e_api::none: PYRO_CORE_ASSERT(false, "[vertex_buffer] e_renderer_api::none currently not supported!"); return nullptr;
    case renderer_api::e_api::open_gl: return make_ref<gl_index_buffer>(indices, count);
    }

    PYRO_CORE_ASSERT(false, "[vertex_buffer] Unknown renderer api!");
    return nullptr;
}

//=================== buffer layout =======================================

pyro::buffer_layout::buffer_layout(std::initializer_list<buffer_element> elements)
    :m_elements(elements)
{
    calculate_offset_and_stride();
}

const std::vector<pyro::buffer_element>& pyro::buffer_layout::elements() const
{
    return m_elements;
}

uint32_t pyro::buffer_layout::stride() const
{
    return m_stride;
}

void pyro::buffer_layout::calculate_offset_and_stride()
{
    size_t offset = 0;
    m_stride = 0;
    for(auto& element : m_elements)
    {
        element.offset = offset;
        offset += element.size;
        m_stride += element.size;
    }
}
