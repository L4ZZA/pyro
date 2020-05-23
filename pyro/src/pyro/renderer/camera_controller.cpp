#include "pyro_pch.h"
#include "camera_controller.h"
#include "pyro/core/input.h"
#include "pyro/core/key_codes.h"
#include "pyro/core/application.h"

//====================== 2D CAMERA CONTROLLER ========================

pyro::orthographic_camera_controller::orthographic_camera_controller(
    glm::vec3 const &position,
    float aspect_ratio,
    float zoom_level /*= 1.f*/,
    bool rotation /*= false*/
    )
    : m_zoom_speed(5.0f)
    , m_camera_rotation(0.f)
    , m_camera_translation_speed(1.0f)
    , m_camera_rotation_speed(180.0f)
    , m_aspect_ratio(aspect_ratio)
    , m_zoom_level(zoom_level)
    , m_rotation(rotation)
    , m_camera_position(position)
    , m_bounds{ -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level }
{
	PYRO_PROFILE_FUNCTION();
    m_camera = make_ref<orthographic_camera>(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
}

void pyro::orthographic_camera_controller::on_update(timestep ts)
{
	PYRO_PROFILE_FUNCTION();
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

        m_camera->rotation({0.f,0.f,m_camera_rotation.z});
    }

    m_camera->position(m_camera_position);
    m_camera_translation_speed = m_zoom_level;
}

void pyro::orthographic_camera_controller::on_event(event &e)
{
	PYRO_PROFILE_FUNCTION();
    event_dispatcher dispatcher(e);
    dispatcher.dispatch<mouse_scrolled_event>(BIND_EVENT_FN(orthographic_camera_controller::on_mouse_scrolled));
    dispatcher.dispatch<window_resize_event>(BIND_EVENT_FN(orthographic_camera_controller::on_window_resized));
}

pyro::ref<pyro::camera>
pyro::orthographic_camera_controller::camera() const
{
    return m_camera;
}

void pyro::orthographic_camera_controller::zoom_level(float level)
{
    m_zoom_level = level;
    calculate_view();
}

float pyro::orthographic_camera_controller::zoom_level() const
{
    return m_zoom_level;
}

void pyro::orthographic_camera_controller::position(glm::vec3 const &pos)
{
    m_camera_position = pos;
}

void pyro::orthographic_camera_controller::rotation(glm::vec3 const &rot)
{
    m_camera_rotation = rot;
}

void pyro::orthographic_camera_controller::calculate_view()
{
    m_bounds.left = -m_aspect_ratio * m_zoom_level;
    m_bounds.right = m_aspect_ratio * m_zoom_level;
    m_bounds.bottom = -m_zoom_level;
    m_bounds.top = m_zoom_level;
    m_camera->projection_matrix(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
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
        m_camera_rotation.z -= m_camera_rotation_speed * ts;
    }
    else if(rotation == anticlock_wise)
    {
        m_camera_rotation.z += m_camera_rotation_speed * ts;
    }
}

bool pyro::orthographic_camera_controller::on_mouse_scrolled(mouse_scrolled_event &e)
{
	PYRO_PROFILE_FUNCTION();
    m_zoom_level -= e.y_offset() * m_zoom_speed;
    m_zoom_level = std::max(m_zoom_level, m_zoom_speed);
    calculate_view();
    // returns if event is handled.
    return false;
}

bool pyro::orthographic_camera_controller::on_window_resized(window_resize_event &e)
{
	PYRO_PROFILE_FUNCTION();
    m_aspect_ratio = static_cast<float>(e.width()) / static_cast<float>(e.height());
    calculate_view();
    // returns if event is handled.
    return false;
}

//====================== 3D CAMERA CONTROLLER ========================

pyro::perspective_camera_controller::perspective_camera_controller(
    e_control_type control_type,
    float width, float height,
    float fov,
    float near_z, float far_z)
    : m_camera_translation_speed(2.5f)
    , m_camera_rotation_speed(90.0f)
{
	PYRO_PROFILE_FUNCTION();
    m_camera = make_ref<perspective_camera>(width, height, fov, near_z, far_z);
    m_camera_position = m_camera->position();
    m_camera_rotation = m_camera->rotation();
    s_control_type = control_type;
    if(s_control_type == e_control_type::first_person)
        application::window().hide_mouse_cursor();
}

void pyro::perspective_camera_controller::on_update(timestep ts)
{
	PYRO_PROFILE_FUNCTION();
    if(input::key_pressed(pyro::key_codes::KEY_A)) // left 
        move(perspective_camera::e_direction::left, ts);
    else if(input::key_pressed(pyro::key_codes::KEY_D)) // right 
        move(perspective_camera::e_direction::right, ts);

    if(input::key_pressed(pyro::key_codes::KEY_S)) // down 
        move(perspective_camera::e_direction::backward, ts);
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_W)) // up 
        move(perspective_camera::e_direction::forward, ts);

    auto [mouse_delta_x, mouse_delta_y] = input::mouse_position();
    process_mouse(s_control_type, mouse_delta_x, mouse_delta_y);

    m_camera->update_camera_vectors();

}

void pyro::perspective_camera_controller::on_event(event &e)
{
	PYRO_PROFILE_FUNCTION();
}

pyro::ref<pyro::camera>
pyro::perspective_camera_controller::camera() const
{
    return m_camera;
}

void pyro::perspective_camera_controller::zoom_level(float level)
{
    // TODO: some how translate to degrees.
    m_camera->fov(level);
}

float pyro::perspective_camera_controller::zoom_level() const
{
    return m_camera->fov();
}

void pyro::perspective_camera_controller::position(glm::vec3 const &pos)
{
    m_camera_position = pos;
}

void pyro::perspective_camera_controller::rotation(glm::vec3 const &rot)
{
    m_camera_rotation = rot;
}

void pyro::perspective_camera_controller::process_mouse(
    e_control_type control_type,
    float mouse_x, float mouse_y,
    bool constrain_pitch)
{
	PYRO_PROFILE_FUNCTION();
    if(control_type == e_control_type::first_person)
        process_mouse_delta(mouse_x, mouse_y, constrain_pitch);
    else if(control_type == e_control_type::editor)
        process_mouse_panning(mouse_x, mouse_y);
}

void pyro::perspective_camera_controller::process_mouse_delta(
    float mouse_delta_x, float mouse_delta_y,
    bool constrain_pitch)
{
	PYRO_PROFILE_FUNCTION();
    if(mouse_delta_x == 0.f && mouse_delta_y == 0.f)
        return;

    // Smoothen the values
    mouse_delta_x *= s_mouse_sensitivity;
    mouse_delta_y *= s_mouse_sensitivity;

    float &yaw = m_camera_rotation.y;
    float &pitch = m_camera_rotation.x;
    yaw += mouse_delta_x;
    pitch += mouse_delta_y;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if(constrain_pitch)
    {
        static const float pitch_limit = 89.0f;
        if(pitch > pitch_limit)
            pitch = pitch_limit;
        if(pitch < -pitch_limit)
            pitch = -pitch_limit;
    }

    m_camera->rotation(m_camera_rotation);
}

void pyro::perspective_camera_controller::process_mouse_panning(float mouse_x, float mouse_y)
{
	PYRO_PROFILE_FUNCTION();
}

void pyro::perspective_camera_controller::move(perspective_camera::e_direction direction, timestep ts)
{
	PYRO_PROFILE_FUNCTION();
    //m_camera->move(direction, ts);
    if(direction == perspective_camera::forward)
        m_camera_position += m_camera_translation_speed * ts * m_camera->front();
    else if(direction == perspective_camera::backward)
        m_camera_position -= m_camera_translation_speed * ts * m_camera->front();

    if(direction == perspective_camera::left)
        m_camera_position -= m_camera_translation_speed * ts * glm::normalize(cross(m_camera->front(), m_camera->up()));
    else if(direction == perspective_camera::right)
        m_camera_position += m_camera_translation_speed * ts * glm::normalize(cross(m_camera->front(), m_camera->up()));

    m_camera->position(m_camera_position);
}

void pyro::perspective_camera_controller::rotate(perspective_camera::e_rotation rotation, perspective_camera::e_axis rotation_axis, timestep ts)
{
	PYRO_PROFILE_FUNCTION();
    //m_camera->rotate(rotation, rotation_axis, ts);
    float *angle_ref = nullptr;
    switch(rotation_axis)
    {
        case x: angle_ref = &m_camera_rotation.x; break;
        case y: angle_ref = &m_camera_rotation.y; break;
        case z: angle_ref = &m_camera_rotation.z; break;
    }

    PYRO_ASSERT(angle_ref, "angle ptr is null!")

        float &angle = *angle_ref;
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

    m_camera->rotation(m_camera_rotation);
}
