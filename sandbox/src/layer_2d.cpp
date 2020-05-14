#include "layer_2d.h"
#include "imgui/imgui.h"

#include "utils/noise.h"
#include "utils/random.h"

using namespace utils;

layer_2d::layer_2d(float width, float height)
    : imgui_layer("Sandbox2D")
    , m_2d_camera_controller({ 157.f, 259.f, 0.f }, width / height, 270.f)
    , m_seed(1)
    , m_other_noise(m_seed)
{
}

layer_2d::~layer_2d()
{
}


void layer_2d::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();

    random::init(m_seed);

    {
        pyro::texture_parameters params;
        params.wrap = pyro::e_texture_wrap::repeat;
        m_checkerboard_texture = pyro::texture_2d::create_from_file("assets/textures/checkerboard.png", params);
    }
    {
        pyro::texture_parameters params;
        params.format = pyro::e_texture_format::red;
        params.filter = pyro::e_texture_filter::nearest;
        params.wrap = pyro::e_texture_wrap::clamp_to_edge;
        m_noise_texture = pyro::texture_2d::create(s_texture_size, s_texture_size, params);
    }
    {
        pyro::texture_parameters params;
        params.format = pyro::e_texture_format::red;
        m_my_texture = pyro::texture_2d::create(s_texture_size, s_texture_size, params);
    }
    
    // populating seed array
    reset_noise_seed(e_noise_type::one_d);
    reset_noise_seed(e_noise_type::two_d);
}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

void layer_2d::reset_noise_seed(e_noise_type const& noise_type)
{
    switch (noise_type)
    {
    case e_noise_type::one_d:
        for (int i = 0; i < s_texture_size; i++)
        {
            m_noise1d_seed[i] = random::get_float();
        }
        m_noise1d_seed[0] = 0.5f; // this will make generation always average around 0.5, since the first octave will be sampling from this value
        break;
    case e_noise_type::two_d:
        for (int i = 0; i < s_texture_size * s_texture_size; i++)
        {
            m_noise2d_seed[i] = random::get_float();
        }
        m_noise1d_seed[0] = 0.5f;
        break;
    }
    
    m_noise_changed = true;
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller.on_update(ts);
    if (m_seed_changed)
    {
        reset_noise_seed(e_noise_type::one_d);
        reset_noise_seed(e_noise_type::two_d);
        m_other_noise.change_seed(m_seed);
        m_noise_changed = true;
        m_seed_changed = false;
    }
    if (m_noise_changed)
    {
        m_noise_changed = false;
        if (m_octaves > 8)
            m_octaves = 8;
        if (m_octaves < 1)
            m_octaves = 1;
        if (m_bias < 0.2f)
            m_bias = 0.2f;

        perlin_noise_1d(s_texture_size, m_octaves, m_bias, m_noise1d_seed.data(), m_noise_1d.data());
        perlin_noise_2d(s_texture_size, m_octaves, m_bias, m_noise2d_seed.data(), m_noise_2d.data());

        m_noise_texture->data(m_noise_2d.data(), m_noise_2d.size(), pyro::e_texture_data_type::Float);

        // other noise 
        // Visit every pixel of the image and assign a color generated with Perlin noise 
        int pos = 0;
        for (unsigned int j = 0; j < s_texture_size; ++j)
        {     // x 
            for (unsigned int i = 0; i < s_texture_size; ++i)
            {  // y 
                float x = static_cast<float>(j) / (static_cast<float>(s_texture_size));
                float y = static_cast<float>(i) / (static_cast<float>(s_texture_size));

                // Typical Perlin noise 
                float n = m_other_noise.noise(m_scale * x, m_scale * y, m_something);

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

void layer_2d::on_imgui_render()
{
    pyro::renderer_2d::reset_stats();
    {
        // Pre Render
        PYRO_PROFILE_SCOPE("layer_2d::pre_render");
        pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1.f});
        pyro::render_command::clear();
    }
    int width = s_texture_size;
    int height = s_texture_size / 4;
    int step = 1;
    float rect_width = 2.f;
    float rect_heigth_max = 2.f;
    float gap_width = step - rect_width;
    float gap = 0.5f;
    {
        // Render
        PYRO_PROFILE_SCOPE("layer_2d::render");
        pyro::renderer_2d::begin_scene(m_2d_camera_controller.camera());

        //for (int x = 0; x < width; x += step)
        //{
        //    float rect_heigth = (m_noise_1d[x] * (float)rect_heigth_max / 2.0f) + (float)rect_heigth_max / 2.0f;
        //    //float rect_heigth = m_noise_1d.at(x);
        //    pyro::quad_properties props;
        //    props.color = { 1.f, 0.0f, 0.0, 0.7f };
        //    float x_pos = x - (x * gap_width);
        //    float y_offset = (rect_heigth / 2) + 0.5;
        //    props.position = { x_pos, y_offset - rect_heigth_max, 0.f };
        //    props.size = { rect_width, rect_heigth };
        //    pyro::renderer_2d::draw_quad(props);
        //}

        //pyro::renderer_2d::current_shader()->set_int("u_grayscale", false);
        //{
        //    pyro::quad_properties props;
        //    props.color = { 1.f, 0.2f, 0.3f,1.f };
        //    props.size = { 500.f, 500.f };
        //    props.texture = m_checkerboard_texture;
        //    pyro::renderer_2d::draw_quad(props);
        //}

        for (int x = 0; x < s_texture_size; x += step)
        for (int y = 0; y < s_texture_size; y += step)
            {
                //float rect_heigth = m_noise_1d.at(x);
                pyro::quad_properties props;
                uint32_t index = y * s_texture_size + x;
                float noise1 = m_noise_2d[index];
                float noise2 = m_vendor_noise_2d[index];

                props.color = color_map(noise1);
                //props.color = { noise ,noise ,noise, 1.f };
                props.position = { x * (rect_width), y * (rect_width), 0.1f };
                props.size = { rect_width, rect_width };
                pyro::renderer_2d::draw_quad(props);

                props.position = { - 256.f - x * (rect_width), y * (rect_width), 0.1f };
                props.color = color_map(noise2);
                pyro::renderer_2d::draw_quad(props);
            }
        
    }
    pyro::renderer_2d::current_shader()->set_int("u_grayscale", true);
    {
        pyro::quad_properties props;
        props.position = { -128.f - gap, 128.f, 0.1f };
        props.color = { 1.f, 1.0f, 1.f, 1.f };
        props.size = { 256.f, 256.f };
        props.texture = m_noise_texture;
        pyro::renderer_2d::draw_quad(props);
    }
    {
        pyro::quad_properties props;
        props.position = { -128.f - gap, 256.f * 1.5f + gap, 0.1f };
        props.color = { 1.f, 1.0f, 1.f, 1.f };
        props.size = { 256.f, 256.f };
        props.texture = m_my_texture;
        pyro::renderer_2d::draw_quad(props);
    }
    pyro::renderer_2d::end_scene();

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
    ImGui::Text("- Seed: ");
    ImGui::SameLine(); 
    m_seed_changed |= ImGui::InputInt("##seed", &m_seed);
    ImGui::Text("- Scale: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderInt("##scale", &m_scale, 1, 100);
    ImGui::Text("- something: ");
    ImGui::SameLine();
    m_noise_changed |= ImGui::SliderFloat("##something", &m_something, 0.1f, 50.f);
    ImGui::Text("- Octaves: ");
    ImGui::SameLine(); 
    m_noise_changed |= ImGui::SliderInt("##octaves", &m_octaves, 1, 8);
    ImGui::Text("- Bias: ");
    ImGui::SameLine(); 
    m_noise_changed |= ImGui::SliderFloat("##bias", &m_bias, 0.1f, 2.f);
    ImGui::Text("- Line width: %f", rect_width);
    ImGui::Text("---------------------");

    auto const& camera = m_2d_camera_controller.camera();
    ImGui::Text("-- Camera:");
    ImGui::Text("- Position: [%f,%f,%f]", camera.position().x, camera.position().y, camera.position().z);
    ImGui::Text("- Zoom: %f", m_2d_camera_controller.zoom_level());
    ImGui::Text("---------------------");
    
    ImGui::End();
}

void layer_2d::on_event(pyro::event &e)
{
    m_2d_camera_controller.on_event(e);


    pyro::event_dispatcher dispatcher(e);
    // dispatch event on window X pressed 
    dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(layer_2d::on_key_pressed));
}


bool layer_2d::on_key_pressed(pyro::key_pressed_event& event)
{
    if (event.event_type() == pyro::event_type_e::key_pressed)
    {
        if (event.key_code() == pyro::key_codes::KEY_R)
        {
            reset_noise_seed(e_noise_type::one_d);
            reset_noise_seed(e_noise_type::two_d);
        }
        if (event.key_code() == pyro::key_codes::KEY_DOWN)
        {
            m_octaves--;
            m_noise_changed = true;
        }
        else if (event.key_code() == pyro::key_codes::KEY_UP)
        {
            m_octaves++;
            m_noise_changed = true;
        }
        if (event.key_code() == pyro::key_codes::KEY_LEFT)
        {
            if (m_bias > 0.2f)
            {
                m_bias -= 0.2f;
                m_noise_changed = true;
            }
        }
        else if (event.key_code() == pyro::key_codes::KEY_RIGHT)
        {
            m_bias += 0.2f;
            m_noise_changed = true;
        }

        
        //PYRO_DEBUG("{0}", static_cast<char>(e.key_code())); 
    }
    return false;
}

glm::vec4 layer_2d::color_map(float noise)
{
    glm::vec4 color;

    // blue
    color = { 0.1f, 0.1f, 0.8, 1.0f };
    // light blue
    if (noise > 0.1f)
        color = { 0.3f, 0.3, 0.9f, 1.0f };
    // green
    if (noise > 0.25f)
        color = { 0.2f, 0.8, 0.2f, 1.0f };
    // light brown
    if (noise > 0.45f)
        color = { .46f, 0.35f, 0.3f, 1.0f };
    // brown
    if (noise > 0.65f)
        color = { .36f, 0.25f, 0.2f, 1.0f };
    // white
    if (noise > 0.85f)
        color = { 1.f, 1.f, 1.f, 1.0f };

    return color;
}
