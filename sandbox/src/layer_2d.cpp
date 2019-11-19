#include "layer_2d.h"
#include "imgui/imgui.h"

layer_2d::layer_2d() : imgui_layer("Sandbox2D"),
m_2d_camera_controller(1280.0f / 720.0f, true)
{
}

layer_2d::~layer_2d()
{
}

void layer_2d::on_attach()
{
    imgui_layer::on_attach();
}

void layer_2d::on_detach()
{
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    m_2d_camera_controller.on_update(ts);
}

void layer_2d::on_imgui_render()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Squares color", glm::value_ptr(m_rect_color));
    ImGui::End();

    // Render
    pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1});
    pyro::render_command::clear();

    pyro::renderer_2d::begin_scene(m_2d_camera_controller.camera());
    pyro::renderer_2d::draw_quad({-1.0f, 0.0f}, {0.8f, 0.8f}, m_rect_color);
    pyro::renderer_2d::draw_quad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
    //pyro::renderer_2d::draw_quad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture);
    pyro::renderer_2d::end_scene();
}

void layer_2d::on_event(pyro::event &e)
{
    m_2d_camera_controller.on_event(e);
}
