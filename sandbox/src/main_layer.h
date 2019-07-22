#pragma once
#include <pyro.h>

class example_layer : public pyro::imgui_layer
{
public:
    example_layer();

    void on_attach() override;
    void on_detach() override;
    void on_update(pyro::timestep timestep) override;
    void on_imgui_render() override;
    void on_event(pyro::event& event) override;

private:
    std::shared_ptr<pyro::shader>         m_shader{};
    std::shared_ptr<pyro::vertex_array>   m_vertex_array{};

    std::shared_ptr<pyro::shader>         m_blue_shader{};
    std::shared_ptr<pyro::vertex_array>   m_rect_va{};

    pyro::orthographic_camera             m_camera;

    glm::vec3                             m_rect_pos{0};
    const float                           m_rect_speed{1.f};
};

