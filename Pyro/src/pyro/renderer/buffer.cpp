#include "pyro_pch.h"
#include "buffer.h"
#include "renderer.h"
#include "platform/opengl/gl_buffer.h"

pyro::vertex_buffer* pyro::vertex_buffer::create(float* vertices, uint32_t size)
{
    switch (renderer::api())
    {
    case e_renderer_api::none: PYRO_ASSERT(false, "[vertex_buffer] e_renderer_api::none currently not supported!"); return nullptr;
    case e_renderer_api::open_gl: return new gl_vertex_buffer(vertices, size);
    }

    PYRO_ASSERT(false, "[vertex_buffer] Unknown renderer api!");
    return nullptr;
}

pyro::index_buffer* pyro::index_buffer::create(uint32_t* indices, uint32_t count)
{
    switch (renderer::api())
    {
    case e_renderer_api::none: PYRO_ASSERT(false, "[vertex_buffer] e_renderer_api::none currently not supported!"); return nullptr;
    case e_renderer_api::open_gl: return new gl_index_buffer(indices, count);
    }

    PYRO_ASSERT(false, "[vertex_buffer] Unknown renderer api!");
    return nullptr;
}