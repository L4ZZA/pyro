#pragma once
#include <pyro.h>

class layer_3d : public pyro::imgui_layer
{
public:
    layer_3d();

    void on_update(const pyro::timestep& timestep) override;
    void on_imgui_render() override; 
    void on_event(pyro::event& event) override; 

private:
    pyro::ref<pyro::shader>         m_color_shader{}; 
    pyro::ref<pyro::vertex_array>   m_triangle_va{}; 

    //pyro::ref<pyro::shader>         m_flat_color_shader{}; 
    pyro::ref<pyro::shader>         m_textured_shader{}; 
    pyro::ref<pyro::vertex_array>   m_rect_va{}; 
    pyro::ref<pyro::vertex_array>   m_cube_va{}; 
    pyro::ref<pyro::texture_2d>     m_texture{}; 
    pyro::ref<pyro::texture_2d>     m_face_texture{}; 

    //pyro::orthographic_camera_controller m_2d_camera_controller; 
    pyro::perspective_camera        m_3d_camera; 

};
