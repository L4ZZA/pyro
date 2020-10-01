#include "scenes/noise1d_scene.h"
#include "imgui/imgui.h"
#include "utils/noise.h"

namespace noise1d
{
    static float s_min_bias = 0.1f;
    static float s_max_bias = 5.0f;
}

noise1d_scene::noise1d_scene(pyro::ref<pyro::camera_controller> cam_controller)
    : base_noise_scene(cam_controller->camera())
    , m_cam_controller(cam_controller)
    , m_seed(0)
    , m_rand(0)
    , m_octaves(8)
    , m_bias(2.f)
    , m_improved_noise(0)
    , m_play_mode(false)
    , m_map_width (1280 >> 1)
    , m_map_height(720  >> 1)
{
}

noise1d_scene::~noise1d_scene()
{
}

void noise1d_scene::init()
{
    m_cam_controller->position({ 124.f, 45.f, 0.f });
    m_cam_controller->zoom_level(255.f);

    // creating empty slots in vectors
    // it also functions as speed optimization
    m_noise_1d.resize(m_map_width);
    m_surface.resize(m_map_width * m_map_height);

    on_seed_changed();
}

void noise1d_scene::on_update(pyro::timestep const &ts)
{
    if(m_play_mode)
    {
        // update play mode objects
        play_mode_update(ts);
    }
    else
    {
        // update editor
        editor_update(ts);
    }
}

void noise1d_scene::on_render() const
{
    pyro::renderer_2d::begin_scene(m_camera);
    int32_t width = m_map_width;
    int32_t height = m_map_height;

    for(int32_t x = 0; x < width; x++)
    {
        for(int32_t y = 0; y < height; y++)
        {
            pyro::quad_properties props;
            // offsetting position to center the map around the origin (coords [0,0])
            props.position = { static_cast<float>(x - width / 2), static_cast<float>(y - height / 2), 0.f };

            int32_t cell = m_surface[x * height + y];
            switch(cell)
            {
            case 0: props.color = { 0.2f, 0.8, 0.2f, 1.0f }; break; // green
            case 1: props.color = { 0.3f, 0.3, 0.8f, 1.0f }; break; // light blue
            }
            pyro::renderer_2d::draw_quad(props);
        }
    }
    pyro::renderer_2d::end_scene();
}

void noise1d_scene::on_imgui_render()
{
    ImGui::Text("- Map width:  %d", m_map_width);
    ImGui::Text("- Map height: %d", m_map_height);
    ImGui::Text("---------------------");

    ImGui::Text("-- Noise:");
    ImGui::Text("- Type: ");
    

    const std::array<char*, 2> items = { "Simple Noise", "Improved Perlin" };
    static const char *current_item = "Simple Noise";
    // The second parameter is the label previewed before opening the combo.
    if(ImGui::BeginCombo("##combo", current_item))
    {
        for(int32_t n = 0; n < items.size(); n++)
        {
            // You can store your selection however you want, outside or inside your objects
            bool is_selected = (current_item == items[n]); 
            if(ImGui::Selectable(items[n], is_selected))
            {
                if(m_noise_type != n)
                {
                    m_noise_type = n;
                    on_seed_changed();
                    current_item = items[n];
                }
            }
        }
        ImGui::EndCombo();
    }
    ImGui::Text("- Seed: ");
    
    if(ImGui::InputInt("##seed", &m_seed))
    {
        on_seed_changed();
    }

    if(m_noise_type == 0)
    {
        ImGui::Text("- Octaves: ");
        
        m_noise_changed |= ImGui::SliderInt("##octaves", &m_octaves, 1, 8);
        ImGui::Text("- Bias: ");
        
        m_noise_changed |= ImGui::SliderFloat("##bias", &m_bias, noise1d::s_min_bias, noise1d::s_max_bias);
    }
    else if(m_noise_type == 1)
    {
        ImGui::Text("- Scale: ");
        
        m_noise_changed |= ImGui::SliderInt("##scale", &m_scale, 1, 100);
        ImGui::Text("- Morph: ");
        
        m_noise_changed |= ImGui::SliderFloat("##morph", &m_morph, 0.1f, 50.f);
        ImGui::Text("- Move x: ");
        
        m_noise_changed |= ImGui::SliderFloat("##move_x", &m_move_x, -50.f, 50.f);
    }
    ImGui::Text("---------------------");
}

void noise1d_scene::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(noise1d_scene::on_key_pressed));
}

void noise1d_scene::on_seed_changed()
{
    m_rand.seed(m_seed);
    m_improved_noise.change_seed(m_seed);
    m_noise_changed = true;
}



bool noise1d_scene::on_key_pressed(pyro::key_pressed_event &e)
{
    //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    return false;
}

glm::vec4 noise1d_scene::color_map(float noise) const
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

void noise1d_scene::create_map()
{
    int32_t width  = m_map_width;
    int32_t height = m_map_height;

    for(int32_t x = 0; x < width; x++)
    {
        for(int32_t y = 0; y < height; y++)
        {
            int32_t index = x * height + y;
            // [0,0] is bottom left
            if(y < m_noise_1d[x] * m_map_height)
                m_surface[index] = 0; // land
            else
                m_surface[index] = 1; // sky
        }
    }
}

void noise1d_scene::editor_update(pyro::timestep const &ts)
{
    if(m_noise_changed)
    {
        m_noise_changed = false;
        if(m_octaves > 8)
            m_octaves = 8;
        if(m_octaves < 1)
            m_octaves = 1;
        if(m_bias < noise1d::s_min_bias)
            m_bias = noise1d::s_min_bias;

        if(m_noise_type == 0)
        {
            utils::perlin_noise_1d(
                m_map_width, 
                m_octaves, 
                m_bias, 
                m_seed, 
                m_noise_1d.data(), 0.5f);
        }
        else if(m_noise_type == 1)
        {
            auto tmp_vec = m_improved_noise.noise_1d_array(
                m_map_width,
                m_scale,
                m_morph,
                m_move_x);
            std::copy_n(std::move(tmp_vec.begin()), tmp_vec.size(), m_noise_1d.begin());
        }

        create_map();
    }
}

void noise1d_scene::play_mode_update(pyro::timestep const &ts)
{ 
}
