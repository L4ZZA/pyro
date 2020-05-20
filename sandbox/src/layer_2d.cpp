#include "layer_2d.h"
#include "imgui/imgui.h"


layer_2d::layer_2d(float width, float height)
    : imgui_layer("Sandbox2D")
    , m_2d_camera_controller({ 0.f,0.f,0.f }, width / height, 10.f)
{
    auto cam = m_2d_camera_controller.camera();
    m_scene1 = pyro::make_ref<noise1d_scene>(cam);
}

layer_2d::~layer_2d()
{
}


void layer_2d::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    m_scene1->init();
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
    m_scene1->on_update(ts);
}

void layer_2d::on_render() const
{
    pyro::renderer_2d::reset_stats();
    {
        // Render
        PYRO_PROFILE_SCOPE("layer_2d::render");
        m_scene1->on_render();
    }
}

void layer_2d::on_imgui_render()
{
    ImGui::Begin("Settings");

    //ImGui::ColorEdit3("Squares color", glm::value_ptr(m_rect_color));
    //for (auto& result : m_profile_results)
    //{
    //    char label[50];
    //    strcpy_s(label, "%.3fms ");
    //    strcat_s(label, result.name);
    //    ImGui::Text(label, result.time);
    //}
    //m_profile_results.clear();

    auto stats = pyro::renderer_2d::stats();
    ImGui::Text("-- 2D Renderer stats:");
    ImGui::Text("- Draw calls: %d", stats.draw_calls);
    ImGui::Text("- Quads: %d", stats.quad_count);
    ImGui::Text("- Vertices: %d", stats.total_vertex_count());
    ImGui::Text("- Indices: %d", stats.total_index_count());
    ImGui::Text("---------------------");

    ImGui::Text("-- Noise:");
    //ImGui::Text("- Seed: ");
    //ImGui::SameLine(); 
    //m_seed_changed |= ImGui::InputInt("##seed", &m_seed);
    //ImGui::Text("- Scale: ");
    //ImGui::SameLine();
    //m_noise_changed |= ImGui::SliderInt("##scale", &m_scale, 1, 100);
    //ImGui::Text("- Morph: ");
    //ImGui::SameLine();
    //m_noise_changed |= ImGui::SliderFloat("##morph", &m_morph, 0.1f, 50.f);
    //ImGui::Text("- Move x: ");
    //ImGui::SameLine();
    //m_noise_changed |= ImGui::SliderFloat("##move_x", &m_move_x, -50.f, 50.f);
    //ImGui::Text("- Move y: ");
    //ImGui::SameLine();
    //m_noise_changed |= ImGui::SliderFloat("##move_y", &m_move_y, -50.f, 50.f);
    //ImGui::Text("- Octaves: ");
    //ImGui::SameLine(); 
    //m_noise_changed |= ImGui::SliderInt("##octaves", &m_octaves, 1, 8);
    //ImGui::Text("- Bias: ");
    //ImGui::SameLine(); 
    //m_noise_changed |= ImGui::SliderFloat("##bias", &m_bias, 0.1f, 2.f);
    //ImGui::Text("- Line width: %f", rect_width);
    ImGui::Text("---------------------");

    pyro::ref<pyro::camera> camera = m_2d_camera_controller.camera();
    ImGui::Text("-- Camera:");
    ImGui::Text("- Position: [%f,%f,%f]", camera->position().x, camera->position().y, camera->position().z);
    ImGui::Text("- Zoom: %f", m_2d_camera_controller.zoom_level());
    ImGui::Text("---------------------");

    ImGui::End();
}

void layer_2d::on_event(pyro::event &e)
{
    m_2d_camera_controller.on_event(e);


    pyro::event_dispatcher dispatcher(e);
    // dispatch event on window X pressed 
    //dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(noise1d_scene::on_key_pressed));
}

