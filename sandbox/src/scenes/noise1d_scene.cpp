#include "scenes/noise1d_scene.h"
#include "imgui/imgui.h"
#include "utils/noise.h"
#include "utils/random.h"

noise1d_scene::noise1d_scene(pyro::ref<pyro::camera> const &camera)
    : base_noise_scene(camera)
    , m_octaves(5)
{
}

noise1d_scene::~noise1d_scene()
{
}

void noise1d_scene::init()
{
    on_seed_changed();
}

void noise1d_scene::deinit()
{
}

void noise1d_scene::on_update(pyro::timestep const &ts)
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

        utils::perlin_noise_1d(
            s_texture_size, 
            m_octaves, m_bias, m_noise1d_seed.data(), m_noise_1d.data());
    }
}

void noise1d_scene::on_render_internal() const
{
    int width = s_texture_size;
    int height = s_texture_size / 4;
    int step = 1;
    float line_rect_width = 0.2f;
    float rect_heigth_max = 10.f;
    float gap_width = step - rect_width;
    float gap = 0.0f;

    for (int x = 0; x < width; x += step)
    {
        float rect_heigth = 
            (m_noise_1d[x] * static_cast<float>(rect_heigth_max) / 2.0f) 
            + static_cast<float>(rect_heigth_max) / 2.0f;
        //float rect_heigth = m_noise_1d.at(x);
        pyro::quad_properties props;
        props.color = { 1.f, 0.0f, 0.0, 0.7f };
        float x_pos = x - (x * gap_width);
        float y_offset = (rect_heigth / 2) + 0.5;
        props.position = { x_pos, y_offset - rect_heigth_max, 0.f };
        props.size = { line_rect_width, rect_heigth };
        pyro::renderer_2d::draw_quad(props);
    }
}

void noise1d_scene::on_imgui_render()
{
    ImGui::Text("- Octaves: ");
    ImGui::SameLine(); 
    m_noise_changed |= ImGui::SliderInt("##octaves", &m_octaves, 1, 8);
    ImGui::Text("- Bias: ");
    ImGui::SameLine(); 
    m_noise_changed |= ImGui::SliderFloat("##bias", &m_bias, 0.1f, 2.f);
    ImGui::Text("- Line width: %f", rect_width);
}

void noise1d_scene::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(noise1d_scene::on_key_pressed));
}


bool noise1d_scene::on_key_pressed(pyro::key_pressed_event &e)
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

void noise1d_scene::on_seed_changed()
{
    for(int i = 0; i < s_texture_size; i++)
    {
        m_noise1d_seed[i] = utils::random::get_float();
    }
    //m_noise1d_seed[0] = 0.5f; // this will make generation always average around 0.5, since the first octave will be sampling from this value 

    m_noise_changed = true;
}
