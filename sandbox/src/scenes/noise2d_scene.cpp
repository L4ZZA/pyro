#include "scenes/noise2d_scene.h"
#include "imgui/imgui.h"
#include "utils/noise.h"

noise2d_scene::noise2d_scene(pyro::ref<pyro::camera> const &camera)
    : base_noise_scene(camera)
    , m_seed(0)
    , m_rand(0)
    , m_other_noise(0)
{
}

noise2d_scene::~noise2d_scene()
{
}

void noise2d_scene::init()
{
    pyro::texture_parameters params;
    params.format = pyro::e_texture_format::red;
    params.filter = pyro::e_texture_filter::nearest;
    m_noise_texture = pyro::texture_2d::create(s_texture_size, s_texture_size, params);
    
    m_my_texture = pyro::texture_2d::create(s_texture_size, s_texture_size, params);
    
    on_seed_changed();
}

void noise2d_scene::deinit()
{
}

void noise2d_scene::on_update(pyro::timestep const &ts)
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

        utils::perlin_noise_2d(
            s_texture_size, m_octaves, m_bias, m_seed, m_noise_2d.data());

        m_noise_texture->data(m_noise_2d.data(), m_noise_2d.size(), pyro::e_texture_data_type::Float);

        // other noise 
        // Visit every pixel of the image and assign a color generated with Perlin noise 
        int pos = 0;
        for(unsigned int y = 0; y < s_texture_size; ++y)
        {     // x 
            for(unsigned int x = 0; x < s_texture_size; ++x)
            {  // y 
                float dx = static_cast<float>(x) / (static_cast<float>(s_texture_size));
                float dy = static_cast<float>(y) / (static_cast<float>(s_texture_size));

                // Typical Perlin noise 
                float n = m_other_noise.noise(m_scale * dx - m_move_x, m_scale * dy + m_move_y, m_morph);

                //// Wood like structure 
                //n = 20 * m_other_noise.noise(x, y, m_something);
                //n = n - floor(n);

                m_vendor_noise_2d[pos] = n;
                pos++;
            }
        }
        m_my_texture->data(m_vendor_noise_2d.data(), m_vendor_noise_2d.size(), pyro::e_texture_data_type::Float);
    }
}

void noise2d_scene::on_render_internal() const
{
    for(int y = 0; y < s_texture_size; y += step)
        for(int x = 0; x < s_texture_size; x += step)
        {
            //float rect_heigth = m_noise_1d.at(x);
            pyro::quad_properties props;
            uint32_t index = y * s_texture_size + x;
            float noise1 = m_noise_2d[index];
            float noise2 = m_vendor_noise_2d[index];

            props.color = color_map(noise1);
            //props.color = { noise ,noise ,noise, 1.f };
            props.position = { x * (rect_width), y * (rect_width), 0.0f };
            props.size = { rect_width, rect_width };
            pyro::renderer_2d::draw_quad(props);

            props.position = { - (s_texture_size * step) + x * (rect_width),  y * (rect_width), 0.0f };
            props.color = color_map(noise2);
            pyro::renderer_2d::draw_quad(props);
        }

    pyro::renderer_2d::current_shader()->set_int("u_grayscale", true);
    {
        pyro::quad_properties props;
        props.position = { rect_width * s_texture_size * 1.5f, rect_width * s_texture_size * .5f, 0.1f };
        props.color = { 1.f, 1.0f, 1.f, 1.f };
        props.size = { rect_width * s_texture_size, rect_width * s_texture_size };
        props.texture = m_noise_texture;
        pyro::renderer_2d::draw_quad(props);
    }
    {
        pyro::quad_properties props;
        props.position = { -rect_width * s_texture_size * 1.5f, rect_width * s_texture_size * .5f, 0.1f };
        props.color = { 1.f, 1.0f, 1.f, 1.f };
        props.size = { rect_width * s_texture_size, rect_width * s_texture_size };
        props.texture = m_my_texture;
        pyro::renderer_2d::draw_quad(props);
    }
}

void noise2d_scene::on_imgui_render()
{
    ImGui::Text("- Seed: ");
    ImGui::SameLine();
    if(ImGui::InputInt("##seed", &m_seed))
    {
        on_seed_changed();
    }
    ImGui::Text("- Octaves: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderInt("##octaves", &m_octaves, 1, 8);
    ImGui::Text("- Bias: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderFloat("##bias", &m_bias, 0.1f, 2.f);

    ImGui::Text("- Scale: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderInt("##scale", &m_scale, 1, 100);
    ImGui::Text("- Morph: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderFloat("##morph", &m_morph, 0.1f, 50.f);
    ImGui::Text("- Move x: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderFloat("##move_x", &m_move_x, -50.f, 50.f);
    ImGui::Text("- Move y: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderFloat("##move_y", &m_move_y, -50.f, 50.f);

    ImGui::Text("- Line width: %f", rect_width);
}

void noise2d_scene::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(noise2d_scene::on_key_pressed));
}


bool noise2d_scene::on_key_pressed(pyro::key_pressed_event &e)
{
    if(e.event_type() == pyro::event_type_e::key_pressed)
    {
        if(e.key_code() == pyro::key_codes::KEY_DOWN)
        {
            m_octaves--;
            m_noise_changed = true;
        }
        else if(e.key_code() == pyro::key_codes::KEY_UP)
        {
            m_octaves++;
            m_noise_changed = true;
        }
        if(e.key_code() == pyro::key_codes::KEY_LEFT)
        {
            if(m_bias > 0.2f)
            {
                m_bias -= 0.2f;
                m_noise_changed = true;
            }
        }
        else if(e.key_code() == pyro::key_codes::KEY_RIGHT)
        {
            m_bias += 0.2f;
            m_noise_changed = true;
        }


        //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    }
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
    m_other_noise.change_seed(m_rand.seed());
    m_noise_changed = true;
}
