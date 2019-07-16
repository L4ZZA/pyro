#pragma once
#include <pyro.h>

class example_layer : public pyro::imgui_layer
{
public:
    example_layer();

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_imgui_render() override;
    void on_event(pyro::event& p_event) override;

private:
    std::shared_ptr<pyro::shader>         m_shader{};
    std::shared_ptr<pyro::vertex_array>   m_vertex_array{};

    std::shared_ptr<pyro::shader>         m_blue_shader{};
    std::shared_ptr<pyro::vertex_array>   m_rect_va{};

    pyro::orthographic_camera             m_camera;

};

