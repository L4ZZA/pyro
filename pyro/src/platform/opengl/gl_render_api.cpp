#include "pyro_pch.h"
#include "gl_render_api.h"
#include "glad/glad.h"

void pyro::gl_render_api::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void pyro::gl_render_api::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void pyro::gl_render_api::clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void pyro::gl_render_api::enable_wireframe()
{
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
}

void pyro::gl_render_api::disable_wireframe()
{
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
}

void pyro::gl_render_api::draw_indexed(const ref<vertex_array>& vertex_array)
{
    glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(), GL_UNSIGNED_INT, nullptr);
}

//-------------------------------------------------------------------------------------------------

void pyro::gl_render_api::enable_alpha()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void pyro::gl_render_api::enable_culling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}
