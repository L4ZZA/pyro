#include "layer_2d.h"
#include "imgui/imgui.h"

#include "helpers/perlin_noise.h"

layer_2d::layer_2d() 
    : imgui_layer("Sandbox2D")
    , m_2d_camera_controller(1280.0f / 720.0f)
{
}

layer_2d::~layer_2d()
{
}


void layer_2d::on_attach()
{
    PYRO_PROFILE_FUNCTION();
    imgui_layer::on_attach();
    
    pyro::texture::wrap(pyro::e_texture_wrap::repeat);
    m_checkerboard_texture = pyro::texture_2d::create_from_file("assets/textures/checkerboard.png");
    
    // populating seed array
    for (int i = 0; i < texture_size; i++)
    {
        float rand_val = static_cast<float>(rand());
        float normalized_val =rand_val / static_cast<float>(RAND_MAX);
        noise_seed[i] = normalized_val;
    }

}

void layer_2d::on_detach()
{
    PYRO_PROFILE_FUNCTION();
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PYRO_PROFILE_FUNCTION();
    m_2d_camera_controller.on_update(ts);

    if (octaves > 8)
        octaves = 1;

    helpers::perlin_noise_1d(texture_size, octaves, noise_seed.data(), noise_1d.data());

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
        float rect_width = 0.2;

        float screen_width = 1280.0f;
        float screen_height = 720.0f;
        for (int x = 0.f; x < texture_size; x ++)
        {
            pyro::quad_properties props;
            props.color = { (x + 1.0f) / (float)texture_size, 0.4f, (x + 1.0f) / (float)texture_size, 0.7f };
            props.size = { 0.09f, 0.09f };
            float height = -(noise_1d[x] * 1.f) + 1.0;
            props.position.x = static_cast<float>(x) / texture_size;
            props.size = { 1.f / texture_size, height  };
            pyro::renderer_2d::draw_quad(props);
        }
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
        if (event.key_code() == pyro::key_codes::KEY_LEFT)
            octaves--;
        else if (event.key_code() == pyro::key_codes::KEY_RIGHT)
            octaves++;
        
        //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    }
    return false;
}
