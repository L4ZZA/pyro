#include "pyro.h"
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
    auto current_scene = std::static_pointer_cast<base_noise_scene>(m_scene_manager.current_scene());

    // hide all ui if the scene is being played
    if(current_scene->is_playing())
        return;

    ImGui::Begin("Settings");

    auto stats = pyro::renderer_2d::stats();
    ImGui::Text("-- 2D Renderer stats:");
    ImGui::Text("- Frame time: %f ms", pyro::application::frame_time() * 1000.f);
    ImGui::Text("- FPS: %d/s", pyro::application::fps());
    ImGui::Text("- Draw calls: %d", stats.draw_calls);
    ImGui::Text("- Quads: %d", stats.quad_count);
    ImGui::Text("- Vertices: %d", stats.total_vertex_count());
    ImGui::Text("- Indices: %d", stats.total_index_count());
    ImGui::Text("---------------------");

    pyro::ref<pyro::camera> camera = m_2d_camera_controller->camera();
    ImGui::Text("-- Camera:");
    ImGui::Text("- Position: [%f,%f,%f]", camera->position().x, camera->position().y, camera->position().z);
    ImGui::Text("- Zoom: %f", m_2d_camera_controller->zoom_level());
    ImGui::Text("---------------------");

    ImGui::Text("Select scenes");
    static int scene_index = 0;
    bool pressed = false;
    pressed |= ImGui::RadioButton("1", &scene_index, 0); ImGui::SameLine();
    pressed |= ImGui::RadioButton("2", &scene_index, 1); ImGui::SameLine();
    pressed |= ImGui::RadioButton("3", &scene_index, 2);
    
    if(pressed)
    {
        m_scene_manager.go_to(scene_index);
    }

    ImGui::Text("[Press Q to quit play mode]");
    if(ImGui::Button("PLAY", { 100.f,25.f })) 
    {
        current_scene->play();
    }
    ImGui::Text("---------------------");

    m_scene_manager.on_imgui_render();

    ImGui::End();
}

void layer_2d::on_event(pyro::event &e)
{
    imgui_layer::on_event(e);
    m_2d_camera_controller->on_event(e);
    pyro::event_dispatcher dispatcher(e);
    // dispatch event on window X pressed 
    auto current_scene = std::static_pointer_cast<base_noise_scene>(m_scene_manager.current_scene());
    dispatcher.dispatch<pyro::key_pressed_event>([&](pyro::key_pressed_event ev) {
        if(current_scene->is_playing() && ev.key_code() == pyro::key_codes::KEY_Q)
        {
            current_scene->stop_playing();
        }
        // return if event is handled or not
        return false;
    });

    m_scene_manager.on_event(e);
}

