#pragma once 
#include "pyro.h" 

class layer_2d final : public pyro::layer
{
public:
    layer_2d(uint32_t width, uint32_t height);
    ~layer_2d() override;
    void on_attach() override;
    void on_detach() override;
    void on_update(const pyro::timestep &ts) override;
    void on_render() const override;
    void on_event(pyro::event &event) override;
private:

    pyro::ref<pyro::scene> m_active_scene;
    pyro::entity m_camera_entity;

    //pyro::ref<pyro::frame_buffer> m_framebuffer;
    bool m_viewport_focused = true;
    bool m_viewport_hovered = false;
        
    pyro::ref<pyro::shader>         m_flat_color_shader; 
    pyro::ref<pyro::texture_2d>     m_checkerboard_texture; 
    pyro::ref<pyro::texture_2d>     m_sprite_sheet; 
    pyro::ref<pyro::sub_texture_2d> m_barrel_texture; 
    pyro::ref<pyro::sub_texture_2d> m_tree_texture; 
 
};