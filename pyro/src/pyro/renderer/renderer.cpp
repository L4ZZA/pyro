#include "pyro_pch.h"
#include "renderer.h"

pyro::renderer::scene_data* pyro::renderer::s_scene_data = new scene_data;

void pyro::renderer::init()
{
    render_command::init();
}

void pyro::renderer::begin_scene(camera& camera, const ref<shader>& shader)
{
    shader->bind();
    shader->set_uniform("u_view_projection", s_scene_data->view_projection_matrix);
    s_scene_data->view_projection_matrix = camera.view_projection_matrix();
}

void pyro::renderer::end_scene()
{
}

void pyro::renderer::submit(
    const ref<shader>& shader, 
    const ref<vertex_array>& vertex_array, 
    const glm::mat4& transform /*= glm::mat4(1.f)*/)
{
    shader->set_uniform("u_transform", transform);

    vertex_array->bind();
    render_command::submit(vertex_array);
}
