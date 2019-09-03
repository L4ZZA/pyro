#pragma once
#include <pyro.h>

class example_layer : public pyro::imgui_layer
{
public:
    example_layer();

    void on_attach() override;
    void on_detach() override;
    void on_update(const pyro::timestep& timestep) override;
    void on_imgui_render() override;
    void on_event(pyro::event& event) override;

private:
    pyro::ref<pyro::shader>         m_color_shader{};
    pyro::ref<pyro::vertex_array>   m_triangle_va{};

    pyro::ref<pyro::shader>         m_flat_color_shader{};
    pyro::ref<pyro::shader>         m_textured_shader{};
    pyro::ref<pyro::vertex_array>   m_rect_va{};
    pyro::ref<pyro::vertex_array>   m_cube_va{};
    pyro::ref<pyro::texture_2d>     m_texture{};
    pyro::ref<pyro::texture_2d>     m_face_texture{};

    pyro::orthographic_camera       m_2d_camera;
    pyro::perspective_camera        m_3d_camera;

    glm::vec3                       m_rect_color{.2f, .3f, .6f};
    glm::vec3                       m_rect_pos{0};
    const float                     m_rect_speed{1.f};
};

