﻿#include "pyro_pch.h"
#include "gl_render_api.h"
#include "glad/glad.h"

void pyro::gl_render_api::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void pyro::gl_render_api::clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void pyro::gl_render_api::draw_indexed(const std::shared_ptr<vertex_array>& vertex_array)
{
    glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(), GL_UNSIGNED_INT, nullptr);
}
