#include "editor_layer.h"
#include "imgui/imgui.h"


editor_layer::editor_layer() : imgui_layer("Sandbox2D"),
m_2d_camera_controller(1280.0f / 720.0f, true)
{
}

editor_layer::~editor_layer()
{
}

void editor_layer::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    
    pyro::texture::wrap(pyro::e_texture_wrap::repeat);
    m_checkerboard_texture = pyro::texture_2d::create_from_file("assets/textures/checkerboard.png");
}

void editor_layer::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

static float rotation = 0.0f;
void editor_layer::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller.on_update(ts);
    rotation += ts * 0.5f;
}

void editor_layer::on_imgui_render()
{
    pyro::renderer_2d::reset_stats();
    {
        // Pre Render
        PYRO_PROFILE_SCOPE("editor_layer::pre_render");
        pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1});
        pyro::render_command::clear();
    }
    {
        // Render
        PYRO_PROFILE_SCOPE("editor_layer::render");
        pyro::renderer_2d::begin_scene(m_2d_camera_controller.camera());
        pyro::quad_properties props;
        
        props.position = {1.f, 0.0f, 0.f};
        props.size = {0.8f, 0.85f};
        props.rotation = -45.f;
        props.color = { 0.8f, 0.2f, 0.3f, 1.0f };
        pyro::renderer_2d::draw_quad(props);

        props.position = {-1.0f, 0.0f, 0.f};
        props.size = {0.8f, 0.8f};
        props.rotation = 0.f;
        props.color = {0.8f, 0.3f, 0.2f, 1.0f};
        pyro::renderer_2d::draw_quad(props);

        props.position = {0.5f, -0.5f, 0.f};
        props.size = {0.5f, 0.75f};
        props.color = {m_rect_color, 1.f};
        pyro::renderer_2d::draw_quad(props);

        props.position = { 0.f, 0.f, -0.1f };
        props.size = {20.f, 20.f};
        props.color = glm::vec4(1.f);
        props.tiling_factor = 10.f;
        props.texture = m_checkerboard_texture;
        pyro::renderer_2d::draw_quad(props);

        props.color = glm::vec4(1);
        props.position = { -2.f, 0.f, 0.0f };
        props.size = {1.f, 1.f};
        props.rotation = rotation;
        props.tiling_factor = 20.f;
        props.color = { .8f, 1.f, .8f, 1.f };
        pyro::renderer_2d::draw_quad(props);

        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                pyro::quad_properties props;
                props.color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                props.size = { 0.45f, 0.45f };
                props.position = { x, y, 0.f };
                pyro::renderer_2d::draw_quad(props);
            }
        }
        pyro::renderer_2d::end_scene();
    }

    ImGui::Begin("Settings");

    ImGui::ColorEdit3("Squares color", glm::value_ptr(m_rect_color));
    for (auto& result : m_profile_results)
    {
        char label[50];
        strcpy_s(label, "%.3fms ");
        strcat_s(label, result.name);
        ImGui::Text(label, result.time);
    }
    m_profile_results.clear();

    auto stats = pyro::renderer_2d::stats();
    ImGui::Text("-- 2D Renderer stats:");
    ImGui::Text("- Draw calls: %d", stats.draw_calls);
    ImGui::Text("- Quads: %d", stats.quad_count);
    ImGui::Text("- Vertices: %d", stats.total_vertex_count());
    ImGui::Text("- Indices: %d", stats.total_index_count());
    ImGui::Text("---------------------");
    
    ImGui::End();
}

void editor_layer::on_event(pyro::event &e)
{
    m_2d_camera_controller.on_event(e);
}
