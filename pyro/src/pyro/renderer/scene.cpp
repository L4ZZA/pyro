#include "pyro_pch.h"
#include "pyro/renderer/scene.h"
#include "pyro/renderer/renderer_2d.h"
#include "pyro/renderer/render_command.h"

pyro::scene::scene(
    pyro::ref<pyro::camera> const &camera, 
    glm::vec4 clear_color /*= { 0.1f, 0.1f, 0.1f, 1.f }*/)
    : m_camera(camera)
    , m_clear_color(clear_color)
{
}

void pyro::scene::on_render() const
{
    {
        // Pre Render
        PYRO_PROFILE_SCOPE("scene::pre_render");
        pyro::render_command::clear_color(m_clear_color);
        pyro::render_command::clear();
    }
    renderer_2d::begin_scene(m_camera);
    on_render_internal();
    renderer_2d::end_scene();
}
