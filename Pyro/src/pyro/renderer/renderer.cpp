#include "pyro_pch.h"
#include "renderer.h"

void pyro::renderer::begin_scene()
{
}

void pyro::renderer::end_scene()
{
}

void pyro::renderer::submit(const std::shared_ptr<vertex_array>& vertex_array)
{
    vertex_array->bind();
    render_command::submit(vertex_array);
}
