#include "pyro_pch.h"
#include "renderer.h"

pyro::renderer::scene_data* pyro::renderer::s_scene_data = new scene_data;

void pyro::renderer::begin_scene(camera& camera)
{
    s_scene_data->view_projection_matrix = camera.view_projection_matrix();
}

void pyro::renderer::end_scene()
{
}

void pyro::renderer::submit(const std::shared_ptr<shader>& shader, const std::shared_ptr<vertex_array>& vertex_array)
{
    shader->bind();
    shader->set_uniform("u_view_projection", s_scene_data->view_projection_matrix);
    vertex_array->bind();
    render_command::submit(vertex_array);
}
