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
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    
    pyro::texture::wrap(pyro::e_texture_wrap::repeat);
    m_checkerboard_texture = pyro::texture_2d::create_from_file("assets/textures/checkerboard.png");
}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller.on_update(ts);
}

void layer_2d::on_imgui_render()
{
    {
        // Pre Render
        PYRO_PROFILE_SCOPE("layer_2d::pre_render");
        pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1});
        pyro::render_command::clear();
    }
    {
        // Render
        PYRO_PROFILE_SCOPE("layer_2d::render");
        pyro::renderer_2d::begin_scene(m_2d_camera_controller.camera());
        pyro::quad_properties props;
        props.position = {-1.0f, 0.0f, 0.f};
        props.size = {0.8f, 0.8f};
        props.color = {0.8f, 0.3f, 0.2f, 1.0f};
        pyro::renderer_2d::draw_quad(props);
        
        props.position = {0.5f, -0.5f, 0.f};
        props.size = {0.5f, 0.75f};
        props.color = {m_rect_color, 1.f};
        pyro::renderer_2d::draw_quad(props);

        props.position = {-0.0f, 0.0f,-0.1f};
        props.size = {10.f, 10.f};
        props.color = {.8f, 1.f, .8f, 1.f};
        props.tiling_factor = 10.f;
        props.texture = m_checkerboard_texture;
        pyro::renderer_2d::draw_quad(props);
        pyro::renderer_2d::end_scene();
    }

    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Squares color", glm::value_ptr(m_rect_color));
    for(auto& result : m_profile_results)
    {
        char label[50];
        strcpy_s(label, "%.3fms ");
        strcat_s(label, result.name);
        ImGui::Text(label, result.time);
    }
    m_profile_results.clear();
    ImGui::End();
}

void layer_2d::on_event(pyro::event &e)
{
    m_2d_camera_controller.on_event(e);
}
