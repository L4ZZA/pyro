#include "layer_2d.h"
#include "imgui/imgui.h"
#include "scenes/noise1d_scene.h" 
#include "scenes/noise2d_scene.h" 
#include "scenes/roguelike_scene.h" 
#include "utils/random.h" 


layer_2d::layer_2d(float width, float height)
    : imgui_layer("Sandbox2D")
    , m_seed(0)
    , m_2d_camera_controller(
            new pyro::orthographic_camera_controller(
                glm::vec3{ 0.f,0.f,0.f }, width / height, 10.f))
{
    auto cam = m_2d_camera_controller->camera();
    m_scene_manager.add_scene(pyro::make_ref<noise1d_scene>(m_2d_camera_controller));
    m_scene_manager.add_scene(pyro::make_ref<noise2d_scene>(m_2d_camera_controller));
    m_scene_manager.add_scene(pyro::make_ref<roguelike_scene>(m_2d_camera_controller));
}

layer_2d::~layer_2d()
{
}


void layer_2d::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    m_scene_manager.init_first_scene();
}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller->on_update(ts);
    m_scene_manager.on_update(ts);
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
        m_scene_manager.on_render();
    }
}

void layer_2d::on_imgui_render()
{
    ImGui::Begin("Settings");

    auto stats = pyro::renderer_2d::stats();
    ImGui::Text("-- 2D Renderer stats:");
    ImGui::Text("- Frame time: %f", pyro::application::frame_time());
    ImGui::Text("- FPS: %d/s", pyro::application::fps());
    ImGui::Text("- UPS: %d/s", pyro::application::ups());
    ImGui::Text("- Draw calls: %d", stats.draw_calls);
    ImGui::Text("- Quads: %d", stats.quad_count);
    ImGui::Text("- Vertices: %d", stats.total_vertex_count());
    ImGui::Text("- Indices: %d", stats.total_index_count());
    ImGui::Text("---------------------");

    ImGui::Text("-- Noise:");
    m_scene_manager.on_imgui_render();
    ImGui::Text("---------------------");

    pyro::ref<pyro::camera> camera = m_2d_camera_controller->camera();
    ImGui::Text("-- Camera:");
    ImGui::Text("- Position: [%f,%f,%f]", camera->position().x, camera->position().y, camera->position().z);
    ImGui::Text("- Zoom: %f", m_2d_camera_controller->zoom_level());
    ImGui::Text("---------------------");

    ImGui::End();
}

void layer_2d::on_event(pyro::event &e)
{
    m_2d_camera_controller->on_event(e);
    pyro::event_dispatcher dispatcher(e);
    // dispatch event on window X pressed 
    dispatcher.dispatch<pyro::key_pressed_event>([&](pyro::key_pressed_event ev) {

        switch(ev.key_code())
        {
        case pyro::key_codes::KEY_1: 
            m_scene_manager.go_to(0); 
            break;
        case pyro::key_codes::KEY_2:
            m_scene_manager.go_to(1);
            break;
        case pyro::key_codes::KEY_3:
            m_scene_manager.go_to(2);
            break;
        }

        // return if event is handled or not
        return false;
    });
    m_scene_manager.on_event(e);
}

