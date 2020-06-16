#include "scenes/roguelike_scene.h"
#include "imgui/imgui.h"
#include "utils/noise.h"

roguelike_scene::roguelike_scene(pyro::ref<pyro::camera_controller> cam_controller)
    : base_noise_scene(cam_controller->camera())
    , m_cam_controller(cam_controller)
    , m_seed(0)
    , m_rand(0)
    , m_other_noise(0)
    , m_board_generator(80,50)
{
}

roguelike_scene::~roguelike_scene()
{
}

void roguelike_scene::init()
{
    m_cam_controller->position({ 39.f, 24.f, 0.f });
    m_cam_controller->zoom_level(29.f);

    on_seed_changed();
}

void roguelike_scene::deinit()
{
}

void roguelike_scene::on_update(pyro::timestep const &ts)
{
    if(m_noise_changed)
    {
        m_noise_changed = false;

        int min_rooms = 30;
        int max_rooms = 40;
        int min_room_size = 4;
        int max_room_size = 8;
        m_board_generator.init(m_rand,
                               min_rooms, max_rooms, 
                               min_room_size, max_room_size);
    }

    m_board_generator.on_update(ts);
}


void roguelike_scene::on_render() const
{
    pyro::renderer_2d::begin_scene(m_camera);
    m_board_generator.on_render();
    pyro::renderer_2d::end_scene();
}

void roguelike_scene::on_imgui_render()
{
    ImGui::Text("-- Noise:");
    ImGui::Text("- Seed: ");
    ImGui::SameLine();
    if(ImGui::InputInt("##seed", &m_seed))
    {
        on_seed_changed();
    }
    ImGui::Text("---------------------");

    m_board_generator.on_imgui_render();
}

void roguelike_scene::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(roguelike_scene::on_key_pressed));
    m_board_generator.on_event(e);
}


bool roguelike_scene::on_key_pressed(pyro::key_pressed_event &e)
{   
    return false;
}

glm::vec4 roguelike_scene::color_map(float noise) const
{
    glm::vec4 color;

    // blue
    color = { 0.1f, 0.1f, 0.8, 1.0f };
    // light blue
    if(noise > 0.1f)
        color = { 0.3f, 0.3, 0.9f, 1.0f };
    // green
    if(noise > 0.25f)
        color = { 0.2f, 0.8, 0.2f, 1.0f };
    // light brown
    if(noise > 0.45f)
        color = { .46f, 0.35f, 0.3f, 1.0f };
    // brown
    if(noise > 0.65f)
        color = { .36f, 0.25f, 0.2f, 1.0f };
    // white
    if(noise > 0.85f)
        color = { 1.f, 1.f, 1.f, 1.0f };

    return color;
}

void roguelike_scene::on_seed_changed()
{
    m_rand.seed(m_seed);
    m_other_noise.change_seed(m_rand.seed());
    m_noise_changed = true;
}
