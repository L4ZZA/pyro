#include "layer_2d.h"
#include "imgui/imgui.h"
#include <chrono>

template <typename Fn>
class timer
{
public:
    timer(char const* name, Fn func)
        :m_name(name), m_func(func), m_stopped(false)
    {
        m_start_time_point = std::chrono::high_resolution_clock::now();
    }
    ~timer()
    {
        if(!m_stopped)
            stop();
    }

    void stop()
    {
        const auto end_time_point = std::chrono::high_resolution_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_time_point).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

        m_stopped = true;

        const float duration = (end - start) * 0.001f;
        // calling lambda/callback passing profile_result object
        m_func({m_name, duration});
    }

private:
    char const *m_name;
    Fn m_func;
    bool m_stopped;
    std::chrono::time_point<std::chrono::steady_clock> m_start_time_point;
};

#define PROFILE_SCOPE(name) timer my_timer##__LINE__(name, [&](profile_result res){ m_profile_results.push_back(res); })

layer_2d::layer_2d() : imgui_layer("Sandbox2D"),
m_2d_camera_controller(1280.0f / 720.0f, true)
{
}

layer_2d::~layer_2d()
{
}

void layer_2d::on_attach()
{
    imgui_layer::on_attach();

    m_checkerboard_texture = pyro::texture_2d::create("assets/textures/checkerboard.png");
}

void layer_2d::on_detach()
{
}

void layer_2d::on_update(const pyro::timestep &ts)
{
    // Update
    PROFILE_SCOPE("layer_2d::on_update");
    m_2d_camera_controller.on_update(ts);
}

void layer_2d::on_imgui_render()
{

    {
        // Pre Render
        PROFILE_SCOPE("layer_2d::pre_render");
        pyro::render_command::clear_color({0.1f, 0.1f, 0.1f, 1});
        pyro::render_command::clear();
    }
    {
        // Render
        PROFILE_SCOPE("layer_2d::render");
        pyro::renderer_2d::begin_scene(m_2d_camera_controller.camera());
        pyro::quad_properties props;
        props.position = {-1.0f, 0.0f, 0.f};
        props.size = {0.8f, 0.8f};
        props.color = {0.8f, 0.3f, 0.2f, 1.0f};
        pyro::renderer_2d::draw_quad(props);
        
        props.position = {0.5f, -0.5f, 0.f};
        props.size = {0.5f, 0.75f};
        props.color = {m_rect_color, 1.f};
        pyro::renderer_2d::draw_quad(props);

        props.position = {-0.0f, 0.0f,-0.1f};
        props.size = {10.f, 10.f};
        props.color = {.8f, 1.f, .8f, 1.f};
        props.tiling_factor = 10.f;
        props.texture = m_checkerboard_texture;
        pyro::renderer_2d::draw_quad(props);
        pyro::renderer_2d::end_scene();
    }

    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Squares color", glm::value_ptr(m_rect_color));
    for(auto& result : m_profile_results)
    {
        char label[50];
        strcpy(label, "%.3fms ");
        strcat(label, result.name);
        ImGui::Text(label, result.time);
    }
    m_profile_results.clear();
    ImGui::End();
}

void layer_2d::on_event(pyro::event &e)
{
    m_2d_camera_controller.on_event(e);
}
