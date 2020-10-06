#pragma once
#include "pyro.h"

class camera_controller_ortho : public pyro::scriptable_entity
{
public:
    void on_create() override;
    void on_update(pyro::timestep const &ts) override;
    void on_event(pyro::event &e) override;

private:
    bool on_mouse_scrolled(pyro::mouse_scrolled_event& e);
    bool on_window_resized(pyro::window_resize_event &e);
    bool on_key_pressed(pyro::key_pressed_event &e);

private:
    // Defines the amount of space each mouse-roll moves the camera along 
    // the front (view) vector or the other right (strafe) vector.
    float m_zoom_speed;
    float m_zoom_level;
    float m_width;
    float m_height;

    pyro::camera_component *m_camera_component;
};