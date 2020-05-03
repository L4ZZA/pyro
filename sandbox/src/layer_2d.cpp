#include "layer_2d.h"
#include "imgui/imgui.h"

#include "helpers/perlin_noise.h"
#include "helpers/random.h"

layer_2d::layer_2d() 
    : imgui_layer("Sandbox2D")
    , m_2d_camera_controller({ 31.f, 8.f, 0.f }, 1280.0f / 720.0f, 23.5f)
{
}

layer_2d::~layer_2d()
{
}


void layer_2d::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    
    random::init();

    pyro::texture::wrap(pyro::e_texture_wrap::repeat);
    m_checkerboard_texture = pyro::texture_2d::create_from_file("assets/textures/checkerboard.png");
    
    // populating seed array
    reset_noise_seed();
}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

void layer_2d::reset_noise_seed()
{
    for (int i = 0; i < s_texture_size; i++)
    {
        float rand_val = static_cast<float>(rand());
        float normalized_val = random::get_float();
        m_noise_seed[i] = normalized_val;
    }
    m_noise_seed[0] = 0.5f; // this will make generation always average around 0.5, since the first octave will be sampling from this value
    m_noise_changed = true;
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller.on_update(ts);

    if (m_noise_changed)
    {
        m_noise_changed = false;
        if (m_octaves > 8)
            m_octaves = 8;
        if (m_octaves < 1)
            m_octaves = 1;
        if (m_bias < 0.2f)
            m_bias = 0.2f;

        helpers::perlin_noise_1d(s_texture_size, m_octaves, m_bias, m_noise_seed.data(), m_noise_1d.data());
    }
}

void layer_2d::on_imgui_render()
{
    pyro::renderer_2d::reset_stats();
    {
        // Pre Render
        PYRO_PROFILE_SCOPE("layer_2d::pre_render");
        pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1});
        pyro::render_command::clear();
    }
    {
        // Render
        PYRO_PROFILE_SCOPE("layer_2d::render");
        pyro::renderer_2d::begin_scene(m_2d_camera_controller.camera());
        

        int width  = s_texture_size;
        int height = s_texture_size / 4;
        float step = 1.f;
        float rect_width = 0.1f;
        float rect_heigth_max = 50.f;
        float gap_width = step - rect_width;
        //for (float y = 0; y < height ; y += 1)
        for (int x = 0; x < width; x += step)
        {
            int rect_heigth = (m_noise_1d[x] * (float)rect_heigth_max / 2.0f) + (float)rect_heigth_max / 2.0f;
            pyro::quad_properties props;
            props.color = { 1.f, 0.0f, 0.0, 0.7f };
            props.size = { rect_width, rect_heigth };
            props.position = { x - (x* gap_width), rect_heigth / 2.0, 0.f };
            pyro::renderer_2d::draw_quad(props);
        }

        pyro::quad_properties props;
        props.color = { 0.f, 1.0f, 0.0, 0.7f };
        pyro::renderer_2d::draw_quad(props);
        pyro::renderer_2d::end_scene();
    }

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
    ImGui::Text("- Octaves: %d", m_octaves);
    ImGui::Text("- Bias: %f", m_bias);
    ImGui::Text("- Line width: %f", m_bias);
    ImGui::Text("---------------------");

    ImGui::Text("-- Camera:");
    ImGui::Text("- Octaves: %d", m_octaves);
    ImGui::Text("- Bias: %f", m_bias);
    ImGui::Text("- Line width: %f", m_bias);
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
            reset_noise_seed();

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
            m_bias -= 0.2f;
            m_noise_changed = true;
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
