#include "renderer/scene.h"
#include <pyro/renderer/renderer_2d.h>
#include <pyro/renderer/render_command.h>

scene::scene(pyro::ref<pyro::camera_base> const &camera)
    : m_camera(camera)
{
}

void scene::on_render() const
{
    on_render();
}
