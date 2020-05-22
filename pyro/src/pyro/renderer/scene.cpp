#include "pyro_pch.h"
#include "pyro/renderer/scene.h"
#include "pyro/renderer/renderer_2d.h"
#include "pyro/renderer/render_command.h"

pyro::scene::scene(pyro::ref<pyro::camera> const &camera)
    : m_camera(camera)
{
}

void pyro::scene::on_render() const
{
    renderer_2d::begin_scene(m_camera);
    on_render_internal();
    renderer_2d::end_scene();
}
