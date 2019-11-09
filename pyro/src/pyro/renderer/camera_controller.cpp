﻿#include "pyro_pch.h"
#include "camera_controller.h"
#include "pyro/input.h"
#include "pyro/key_codes.h"
#include "pyro/application.h"

//====================== 2D CAMERA CONTROLLER ========================

pyro::orthographic_camera_controller::orthographic_camera_controller(float aspect_ratio, bool rotation /*= false*/)
    : m_aspect_ratio(aspect_ratio),
      m_zoom_level(1.0f),
      m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level),
      m_rotation(rotation),
      m_camera_position(0, 0, 0),
      m_camera_rotation(0.f),
      m_camera_translation_speed(1.0f),
      m_camera_rotation_speed(180.0f)
{
}

void pyro::orthographic_camera_controller::on_update(timestep ts)
{
    if(input::key_pressed(pyro::key_codes::KEY_A)) // left
        move(e_direction::left, ts);
    else if(input::key_pressed(pyro::key_codes::KEY_D)) // right
        move(e_direction::right, ts);

    if(input::key_pressed(pyro::key_codes::KEY_W)) // up
        move(e_direction::up, ts);
    else if(input::key_pressed(pyro::key_codes::KEY_S)) // down
        move(e_direction::down, ts);

    if(m_rotation)
    {
        
    if(input::key_pressed(pyro::key_codes::KEY_Q)) // up
        rotate(e_rotation::anticlock_wise, e_axis::z, ts);
    else if(input::key_pressed(pyro::key_codes::KEY_E)) // down
        rotate(e_rotation::clock_wise, e_axis::z, ts);

        m_camera.rotation({0.f,0.f,m_camera_rotation});
    }

    m_camera.position(m_camera_position);
	m_camera_translation_speed = m_zoom_level;
}

void pyro::orthographic_camera_controller::on_event(event& e)
{
	event_dispatcher dispatcher(e);
	dispatcher.dispatch<mouse_scrolled_event>(BIND_EVENT_FN(orthographic_camera_controller::on_mouse_scrolled));
	dispatcher.dispatch<window_resize_event>(BIND_EVENT_FN( orthographic_camera_controller::on_window_resized));
}

pyro::camera& pyro::orthographic_camera_controller::camera()
{
    return m_camera;
}

pyro::camera const& pyro::orthographic_camera_controller::camera() const
{
    return m_camera;
}

void pyro::orthographic_camera_controller::zoom_level(float level)
{
    m_zoom_level = level;
}

float pyro::orthographic_camera_controller::zoom_level() const
{
    return m_zoom_level;
}

void pyro::orthographic_camera_controller::move(e_direction direction, timestep ts)
{
    if(direction == up)
        m_camera_position.y += m_camera_translation_speed * ts;
    else if(direction == down)
        m_camera_position.y -= m_camera_translation_speed * ts;

    if(direction == left)
        m_camera_position.x -= m_camera_translation_speed * ts;
    else if(direction == right)
        m_camera_position.x += m_camera_translation_speed * ts;
}

void pyro::orthographic_camera_controller::rotate(e_rotation rotation, e_axis rotation_axis, timestep ts)
{
    if(rotation == clock_wise)
    {
        m_camera_rotation -= m_camera_rotation_speed * ts;
    }
    else if(rotation == anticlock_wise)
    {
        m_camera_rotation += m_camera_rotation_speed * ts;
    }
}

bool pyro::orthographic_camera_controller::on_mouse_scrolled(mouse_scrolled_event& e)
{
    m_zoom_level -= e.y_offset() * 0.25f;
    m_zoom_level = std::max(m_zoom_level, 0.25f);
    m_camera.projection_matrix(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
    // returns if event is handled.
    return false;
}

bool pyro::orthographic_camera_controller::on_window_resized(window_resize_event& e)
{
    m_aspect_ratio = static_cast<float>(e.width()) / static_cast<float>(e.height());
    m_camera.projection_matrix(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
    // returns if event is handled.
    return false;
}

//====================== 3D CAMERA CONTROLLER ========================

pyro::perspective_camera_controller::perspective_camera_controller(
    e_control_type control_type, 
    float width, float height, 
    float fov, 
    float near_z, float far_z)
    : m_camera(fov, width / height, near_z, far_z),
      m_camera_position(0.f, 0.f, 0.f),
      m_camera_rotation(0.f, 0.f, 0.f),
      m_camera_translation_speed(2.5f),
      m_camera_rotation_speed(90.0f)
{
    s_control_type = control_type;
    if(s_control_type == e_control_type::first_person)
        application::window().hide_mouse_cursor();
}

void pyro::perspective_camera_controller::on_update(timestep ts)
{
    if(input::key_pressed(pyro::key_codes::KEY_A)) // left
        move(e_direction::left, ts);
    else if(input::key_pressed(pyro::key_codes::KEY_D)) // right
        move(e_direction::right, ts);

    if(input::key_pressed(pyro::key_codes::KEY_S)) // down
        move(e_direction::backward, ts);
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_W)) // up
        move(e_direction::forward, ts);

    if(m_rotation)
    {
        auto [mouse_delta_x, mouse_delta_y] = input::mouse_position();
        process_mouse(s_control_type, mouse_delta_x, mouse_delta_y);
            
        m_camera.rotation(m_camera_rotation);
    }

    m_camera.position(m_camera_position);
}

void pyro::perspective_camera_controller::on_event(event& e)
{
}

pyro::camera& pyro::perspective_camera_controller::camera()
{
    return m_camera;
}

pyro::camera const& pyro::perspective_camera_controller::camera() const
{
    return m_camera;
}

void pyro::perspective_camera_controller::zoom_level(float level)
{
    m_zoom_level = level;
}

float pyro::perspective_camera_controller::zoom_level() const
{
    return m_zoom_level;
}

void pyro::perspective_camera_controller::process_mouse(
    e_control_type control_type, 
    float mouse_x, float mouse_y,
    bool constrain_pitch)
{
}

void pyro::perspective_camera_controller::process_mouse_delta(
    float mouse_delta_x, float mouse_delta_y,
    bool constrain_pitch)
{
    // Smoothen the values
    mouse_delta_x *= s_mouse_sensitivity;
    mouse_delta_y *= s_mouse_sensitivity;

    m_yaw   += mouse_delta_x;
    m_pitch += mouse_delta_y;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if(constrain_pitch)
    {
        static const float pitch_limit = 89.0f;
        if(m_pitch > pitch_limit)
            m_pitch = pitch_limit;
        if(m_pitch < -pitch_limit)
            m_pitch = -pitch_limit;
    }
}

void pyro::perspective_camera_controller::process_mouse_panning(float mouse_x, float mouse_y)
{
}

void pyro::perspective_camera_controller::move(e_direction direction, timestep ts)
{
    if(direction == forward) 
        m_camera_position += m_camera_translation_speed * ts * m_front_vector; 
    else if(direction == backward) 
        m_camera_position -= m_camera_translation_speed * ts * m_front_vector; 

    if(direction == left) 
        m_camera_position -= m_camera_translation_speed * ts * glm::normalize(cross(m_front_vector, m_up_vector)); 
    else if(direction == right) 
        m_camera_position += m_camera_translation_speed * ts * glm::normalize(cross(m_front_vector, m_up_vector)); 

    //PYRO_CORE_TRACE("3d cam position: [{},{},{}]", m_position.x, m_position.y, m_position.z); 
}

void pyro::perspective_camera_controller::rotate(e_rotation rotation, e_axis rotation_axis, timestep ts)
{
    float* angle_ref = nullptr; 
    switch (rotation_axis) 
    { 
        case x: angle_ref = &m_camera_rotation.x; break; 
        case y: angle_ref = &m_camera_rotation.y; break; 
        case z: angle_ref = &m_camera_rotation.z; break; 
    } 

    PYRO_ASSERT(angle_ref, "angle ptr is null!") 

        float& angle = *angle_ref; 
    if(rotation == anticlock_wise) 
    { 
        angle += m_camera_rotation_speed * ts; 
        if(angle > 360.f) 
        { 
            angle = 0.f; 
            PYRO_CORE_TRACE("A"); 
        } 
    } 
    else if(rotation == clock_wise) 
    { 
        angle -= m_camera_rotation_speed * ts; 
        if(angle < 0.f) 
        { 
            angle = 360.f; 
            PYRO_CORE_TRACE("B"); 
        } 
    } 

    //PYRO_CORE_TRACE("after - 3d cam rotation: [{},{},{}]", m_camera_rotation.x, m_camera_rotation.y, m_camera_rotation.z); 
}