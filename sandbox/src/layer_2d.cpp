#include "layer_2d.h"
#include "imgui/imgui.h"
#include "scenes/noise1d_scene.h" 
#include "scenes/noise2d_scene.h" 


layer_2d::layer_2d(float width, float height)
    : imgui_layer("Sandbox2D")
    , m_2d_camera_controller({ 0.f,0.f,0.f }, width / height, 10.f)
{
    auto cam = m_2d_camera_controller.camera();
    m_scenes.push_back(pyro::make_ref<noise1d_scene>(cam));
    m_scenes.push_back(pyro::make_ref<noise2d_scene>(cam));
}

layer_2d::~layer_2d()
{
}


void layer_2d::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    m_current_scene_index = 0;
    for(auto scene : m_scenes)
        scene->init();
}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
    for(auto &scene : m_scenes)
        scene->deinit();
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller.on_update(ts);
    m_scenes[m_current_scene_index]->on_update(ts);
}

void layer_2d::on_render() const
{
    pyro::renderer_2d::reset_stats();
    {
        // Pre Render
        PYRO_PROFILE_SCOPE("scene::pre_render");
        pyro::render_command::clear_color({ 0.1f, 0.1f, 0.1f, 1.f });
        pyro::render_command::clear();
    }
    {
        // Render
        PYRO_PROFILE_SCOPE("layer_2d::render");
        m_scenes[m_current_scene_index]->on_render();
    }
}

void layer_2d::on_imgui_render()
{
    ImGui::Begin("Settings");

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
    dispatcher.dispatch<pyro::key_pressed_event>([&](pyro::key_pressed_event ev) {

        switch(ev.key_code())
        {
        case pyro::key_codes::KEY_1: 
            m_current_scene_index = 0; 
            break;
        case pyro::key_codes::KEY_2: 
            m_current_scene_index = 1; 
            break;
        }
        // clamp to last scene
        if(m_current_scene_index > m_scenes.size())
            m_current_scene_index = m_scenes.size() - 1;

        m_scenes[m_current_scene_index]->on_event(ev);
        // return if event is handled or not
        return false;
    });
}

