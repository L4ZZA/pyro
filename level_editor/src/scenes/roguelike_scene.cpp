#include "scenes/roguelike_scene.h"
#include "imgui/imgui.h"
#include "utils/noise.h"

roguelike_scene::roguelike_scene(pyro::ref<pyro::camera_controller> cam_controller)
    : base_noise_scene(cam_controller->camera())
    , m_cam_controller(cam_controller)
    , m_rand(0)
    , m_other_noise(0)
    , m_min_rooms(2)
    , m_max_room_tries(1)
    , m_min_room_size(6)
    , m_max_room_size(12)
    , m_seed(0)
    , m_board_generator(80,50)
    , m_play_mode(false)
{
}

roguelike_scene::~roguelike_scene()
{
}

void roguelike_scene::init()
{
    m_cam_controller->position({ 55.f, 24.f, 0.f });
    m_cam_controller->zoom_level(34.f);

    on_seed_changed();
}

void roguelike_scene::deinit()
{
}

void roguelike_scene::on_update(pyro::timestep const &ts)
{
    if(m_play_mode)
        play_mode_update(ts);
    else
        editor_update(ts);
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
    
    if(ImGui::InputInt("##seed", &m_seed))
    {
        on_seed_changed();
    }
    ImGui::Text("---------------------");

    ImGui::Text("- Min rooms: ");
    if(ImGui::SliderInt("##min_rooms", &m_min_rooms, 2, 30))
    {
        on_seed_changed();
    }

    ImGui::Text("- Max tries: [numbers of tries a rooms get regenerated for]");
    if(ImGui::SliderInt("##max_tries", &m_max_room_tries, 0, 20))
    {
        on_seed_changed();
    }

    ImGui::Text("- Min rooms size: ");
    if(ImGui::SliderInt("##min_rooms_size", &m_min_room_size, 4, m_max_room_size))
    {
        on_seed_changed();
    }

    ImGui::Text("- Max rooms size: ");
    if(ImGui::SliderInt("##max_rooms_size", &m_max_room_size, m_min_room_size, 32))
    {
        on_seed_changed();
    }

    m_board_generator.on_imgui_render();
}

void roguelike_scene::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(roguelike_scene::on_key_pressed));
    m_board_generator.on_event(e);
}

void roguelike_scene::on_seed_changed()
{
    // combining the seed with the other parameters we make sure that there is no
    // repetition between rooms if only one of the parameters has changed;
    m_rand.seed(m_seed + m_min_rooms + m_max_room_tries);
    m_other_noise.change_seed(m_rand.seed());
    m_noise_changed = true;
}


void roguelike_scene::editor_update(pyro::timestep const &ts)
{
    if(m_noise_changed)
    {
        m_noise_changed = false;

        m_board_generator.init(m_rand,
            m_min_rooms, m_max_room_tries,
            m_min_room_size, m_max_room_size);
    }

    m_board_generator.on_update(ts);
}

void roguelike_scene::play_mode_update(pyro::timestep const &ts)
{
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
