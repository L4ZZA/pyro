#include "renderer/scene.h"
#include <pyro/renderer/renderer_2d.h>
#include <pyro/renderer/render_command.h>

scene::scene(pyro::ref<pyro::camera> const &camera)
    : m_camera(camera)
{
}

void scene::on_render() const
{
    pyro::renderer_2d::begin_scene(m_camera);
    on_render_internal();
    pyro::renderer_2d::end_scene();
}
