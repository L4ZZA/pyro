#include "scenes/noise2d_scene.h"
#include "utils/imgui_extensions.h"
#include "utils/noise.h"

noise2d_scene::noise2d_scene(pyro::ref<pyro::camera_controller> cam_controller)
    : base_noise_scene(cam_controller->camera())
    , m_cam_controller(cam_controller)
    , m_seed(0)
    , m_rand(0)
    , m_other_noise(0)
    , m_play_mode(false)
    , m_show_texture(false)
{
}

noise2d_scene::~noise2d_scene()
{
}

void noise2d_scene::init()
{
    m_cam_controller->position({ 350.f, 139.5f, 0.f });
    m_cam_controller->zoom_level(205.f);

    m_noise_2d.resize(m_width * m_width);

    pyro::texture_parameters params;
    params.format = pyro::e_texture_format::red;
    params.filter = pyro::e_texture_filter::nearest;
    m_noise_texture =
        pyro::texture_2d::create(m_width, m_width, params);

    on_seed_changed();
}

void noise2d_scene::deinit()
{
}

void noise2d_scene::on_update(pyro::timestep const &ts)
{
    if(m_play_mode)
        play_mode_update(ts);
    else
        editor_update(ts);
}

void noise2d_scene::on_render() const
{
    pyro::renderer_2d::begin_scene(m_camera);
    for(int y = 0; y < m_width; y += m_step)
        for(int x = 0; x < m_width; x += m_step)
        {
            //float rect_heigth = m_noise_1d.at(x);
            pyro::quad_properties props;
            uint32_t index = y * m_width + x;
            float noise1 = m_noise_2d[index];

            props.color = color_map(noise1);
            //props.color = { noise ,noise ,noise, 1.f };
            props.position = { x * (m_rect_width), y * (m_rect_width), 0.0f };
            props.size = { m_rect_width, m_rect_width };
            pyro::renderer_2d::draw_quad(props);
        }
    pyro::renderer_2d::end_scene();

    if(m_show_texture)
    {
        pyro::renderer_2d::begin_scene(m_camera);
        pyro::renderer_2d::current_shader()->set_int("u_grayscale", true);
        {
            pyro::quad_properties props;
            props.position = { m_rect_width * m_width * 1.5f, m_rect_width * m_width * .5f, 0.1f };
            props.color = { 1.f, 1.0f, 1.f, 1.f };
            props.size = { m_rect_width * m_width, m_rect_width * m_width };
            props.texture = m_noise_texture;
            pyro::renderer_2d::draw_quad(props);
        }
        pyro::renderer_2d::end_scene();
    }
}

void noise2d_scene::on_imgui_render()
{

    ImGui::Text("- Show noise texture "); 
    ImGui::ToggleButton("##noise_texture", &m_show_texture);


    ImGui::Text("-- Noise:");
    ImGui::Text("- Type: ");
    ImGui::SameLine();

    const std::array<char *, 2> items = { "Simple Noise", "Improved Perlin" };
    static const char *current_item = "Simple Noise";

    // The second parameter is the label previewed before opening the combo.
    if(ImGui::BeginCombo("##combo", current_item))
    {
        for(int n = 0; n < items.size(); n++)
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
                //if(is_selected)
                //{
                    //// You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                    //ImGui::SetItemDefaultFocus();   
                //}
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
        
        m_noise_changed |= ImGui::SliderFloat("##bias", &m_bias, 0.1f, 2.f);
    }
    else if(m_noise_type == 1)
    {
        ImGui::Text("- Scale: ");
        
        m_noise_changed |= ImGui::SliderInt("##scale", &m_scale, 1, 100);
        ImGui::Text("- Morph: ");
        
        m_noise_changed |= ImGui::SliderFloat("##morph", &m_morph, 0.1f, 50.f);
        ImGui::Text("- Move x: ");
        
        m_noise_changed |= ImGui::SliderFloat("##move_x", &m_move_x, -50.f, 50.f);
        ImGui::Text("- Move y: ");
        
        m_noise_changed |= ImGui::SliderFloat("##move_y", &m_move_y, -50.f, 50.f);
    }

    ImGui::Text("---------------------");
}

void noise2d_scene::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(noise2d_scene::on_key_pressed));
}


void noise2d_scene::editor_update(pyro::timestep const &ts)
{
    if(m_noise_changed)
    {
        m_noise_changed = false;
        if(m_octaves > 8)
            m_octaves = 8;
        if(m_octaves < 1)
            m_octaves = 1;
        if(m_bias < 0.2f)
            m_bias = 0.2f;

        if(m_noise_type == 0)
        {
            utils::perlin_noise_2d(
                m_width, m_octaves, m_bias, m_seed, m_noise_2d.data());

        }
        else if(m_noise_type == 1)
        {
            auto tmp_vec = m_other_noise.noise_2d_array(
                m_width,
                m_scale,
                m_morph,
                m_move_x, m_move_y);
            std::copy_n(std::move(tmp_vec.begin()), tmp_vec.size(), m_noise_2d.begin());

        }
        m_noise_texture->data(m_noise_2d.data(), m_noise_2d.size(),
            pyro::e_texture_data_type::Float);
    }
}

void noise2d_scene::play_mode_update(pyro::timestep const &ts)
{
}

bool noise2d_scene::on_key_pressed(pyro::key_pressed_event &e)
{
    return false;
}

glm::vec4 noise2d_scene::color_map(float noise) const
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

void noise2d_scene::on_seed_changed()
{
    m_rand.seed(m_seed);
    m_other_noise.change_seed(m_seed);
    m_noise_changed = true;
}
