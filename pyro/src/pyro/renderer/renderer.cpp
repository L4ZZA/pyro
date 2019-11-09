#include "pyro_pch.h"
#include "renderer.h"

pyro::renderer::scene_data* pyro::renderer::s_scene_data = new scene_data;

void pyro::renderer::begin_scene(camera& camera, const ref<shader>& shader)
{
    s_scene_data->view_projection_matrix = camera.view_projection_matrix();
    s_scene_data->shader = shader;
    shader->bind();
    shader->set_mat4("u_view_projection", s_scene_data->view_projection_matrix);
}

void pyro::renderer::on_window_resize(uint32_t width, uint32_t height)
{
	render_command::resize_viewport(0, 0, width, height);
}

void pyro::renderer::end_scene()
{
    s_scene_data->shader->unbind(); 
}

void pyro::renderer::submit(
    const ref<shader>& shader, 
    const ref<vertex_array>& vertex_array, 
    const glm::mat4& transform /*= glm::mat4(1.f)*/)
{
    shader->set_mat4("u_transform", transform);

    vertex_array->bind();
    render_command::submit(vertex_array);
}

