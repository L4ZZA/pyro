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
    pyro::ref<pyro::shader>         m_shader{};
    pyro::ref<pyro::vertex_array>   m_vertex_array{};

    pyro::ref<pyro::shader>         m_textured_shader{};
    pyro::ref<pyro::shader>         m_blue_shader{};
    pyro::ref<pyro::vertex_array>   m_rect_va{};

    pyro::orthographic_camera       m_camera;

    glm::vec3                       m_rect_pos{0};
    const float                     m_rect_speed{1.f};
};

