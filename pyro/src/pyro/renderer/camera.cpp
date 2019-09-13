#include "pyro_pch.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "pyro/input.h"
#include "pyro/key_codes.h"
#include "pyro/application.h"


//================== Orthographic Camera [2D] =================================

pyro::orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
    : m_projection_mat(glm::ortho(left, right, bottom, top, -1.f, 1.f))
{
    m_view_projection_mat = m_projection_mat * m_view_mat;
}

void pyro::orthographic_camera::on_update(const timestep& timestep)
{
    if(input::key_pressed(pyro::key_codes::KEY_A)) // left
        move(e_direction::left, timestep);
    else if(input::key_pressed(pyro::key_codes::KEY_D)) // right
        move(e_direction::right, timestep);

    if(input::key_pressed(pyro::key_codes::KEY_W)) // up
        move(e_direction::up, timestep);
    else if(input::key_pressed(pyro::key_codes::KEY_S)) // down
        move(e_direction::down, timestep);
}

void pyro::orthographic_camera::move(e_direction direction, timestep ts)
{
    float speed = s_movement_speed;

    if(direction == up)
        m_position.y += speed * ts;
    else if(direction == down)
        m_position.y -= speed * ts;

    if(direction == left)
        m_position.x -= speed * ts;
    else if(direction == right)
        m_position.x += speed * ts;

    update_view_matrix();
}

void pyro::orthographic_camera::rotate(e_rotation rotation, e_axis, timestep ts)
{
    float speed = s_rotation_speed;

    if(rotation == clock_wise)
    {
        m_rotation -= speed * ts;
    }
    else if(rotation == anticlock_wise)
    {
        m_rotation += speed * ts;
    }

    update_view_matrix();
}

void pyro::orthographic_camera::update_view_matrix()
{
    glm::mat4 transform(1);
    glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0,0,1));
    transform = glm::translate(transform, m_position);

    // inverting the transform matrix 
    m_view_mat = glm::inverse(transform);
    m_view_projection_mat = m_projection_mat * m_view_mat;
}

//================== Perspective Camera [3D] ==================================

pyro::perspective_camera::perspective_camera(
    e_control_type control_type,
    float width, float height, 
    float fov /*= 45.f*/, 
    float near_z /*= 0.1f*/, float far_z /*= 100.f*/) 
    : m_projection_mat(glm::perspective(glm::radians(fov), width / height, near_z, far_z)), 
    m_aspect_ratio(width / height), 
    m_fov(fov), 
    m_near_plane(near_z), 
    m_far_plane(far_z) 
{ 
    s_control_type = control_type;
    if(s_control_type == e_control_type::first_person)
        application::window().hide_mouse_cursor();
    m_position = glm::vec3(0.0f, 0.0f, 3.0f);  
    m_front_vector = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up_vector = glm::vec3(0.0f, 1.0f,  0.0f);
    m_view_mat = glm::lookAt(m_position, m_position + m_front_vector, m_up_vector);

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //m_front_vector = glm::normalize(m_position - cameraTarget);

    //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    //m_righ_vector = glm::normalize(glm::cross(up, m_front_vector));

    //m_up_vector = glm::cross(m_front_vector, m_righ_vector);
    // note that we're translating the scene in the reverse direction of where we want to move 
    //m_view_mat = glm::translate(m_view_mat, start_position);
    m_view_projection_mat = m_projection_mat * m_view_mat; 
    PYRO_CORE_TRACE("3d cam position: [{},{},{}]", m_position.x, m_position.y, m_position.z); 
    PYRO_CORE_TRACE("3d cam rotation: [{},{},{}]", m_rotation_angle.x, m_rotation_angle.y, m_rotation_angle.z); 
}

void pyro::perspective_camera::on_update(const timestep& timestep)
{
    if(input::key_pressed(pyro::key_codes::KEY_A)) // left
        move(e_direction::left, timestep);
    else if(input::key_pressed(pyro::key_codes::KEY_D)) // right
        move(e_direction::right, timestep);

    if(input::key_pressed(pyro::key_codes::KEY_S)) // down
        move(e_direction::backward, timestep);
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_W)) // up
        move(e_direction::forward, timestep);

    auto [mouse_delta_x, mouse_delta_y] = input::mouse_position();
    process_mouse(s_control_type, mouse_delta_x, mouse_delta_y);

    //float delta = input::mouse_scroll();
    //process_mouse_scroll(delta);

    update_camera_vectors();
}

const glm::mat4& pyro::perspective_camera::projection_matrix() const 
{ 
    return m_projection_mat; 
} 

const glm::mat4& pyro::perspective_camera::view_matrix() const 
{ 
    return m_view_mat; 
} 

const glm::mat4& pyro::perspective_camera::view_projection_matrix() const 
{ 
    return m_view_projection_mat; 
}

void pyro::perspective_camera::process_mouse(e_control_type control_type, float mouse_x, float mouse_y, bool constrain_pitch /*= true*/)
{
    if(control_type == e_control_type::first_person)
        process_mouse_delta(mouse_x, mouse_y, constrain_pitch);
    else if(control_type == e_control_type::editor)
        process_mouse_panning(mouse_x, mouse_y);
}

void pyro::perspective_camera::process_mouse_delta(float mouse_delta_x, float mouse_delta_y, bool constrain_pitch /*= true*/)
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

void pyro::perspective_camera::process_mouse_panning(float mouse_x, float mouse_y)
{
}

void pyro::perspective_camera::move(e_direction direction, timestep ts) 
{ 
    if(direction == forward) 
        m_position += s_movement_speed * ts * m_front_vector; 
    else if(direction == backward) 
        m_position -= s_movement_speed * ts * m_front_vector; 

    if(direction == left) 
        m_position -= s_movement_speed * ts * glm::normalize(cross(m_front_vector, m_up_vector)); 
    else if(direction == right) 
        m_position += s_movement_speed * ts * glm::normalize(cross(m_front_vector, m_up_vector)); 

    //PYRO_CORE_TRACE("3d cam position: [{},{},{}]", m_position.x, m_position.y, m_position.z); 
} 

void pyro::perspective_camera::rotate(e_rotation rotation, e_axis rotation_axis, timestep ts) 
{ 
    float* angle_ref = nullptr; 
    switch (rotation_axis) 
    { 
        case x: angle_ref = &m_rotation_angle.x; break; 
        case y: angle_ref = &m_rotation_angle.y; break; 
        case z: angle_ref = &m_rotation_angle.z; break; 
    } 

    PYRO_ASSERT(angle_ref, "angle ptr is null!") 

        float& angle = *angle_ref; 
    if(rotation == anticlock_wise) 
    { 
        angle += s_rotation_speed * ts; 
        if(angle > 360.f) 
        { 
            angle = 0.f; 
            PYRO_CORE_TRACE("A"); 
        } 
    } 
    else if(rotation == clock_wise) 
    { 
        angle -= s_rotation_speed * ts; 
        if(angle < 0.f) 
        { 
            angle = 360.f; 
            PYRO_CORE_TRACE("B"); 
        } 
    } 

    //PYRO_CORE_TRACE("after - 3d cam rotation: [{},{},{}]", m_rotation_angle.x, m_rotation_angle.y, m_rotation_angle.z); 
} 

void pyro::perspective_camera::update_view_matrix() 
{ 
    //glm::mat4 transform{1}; 
    //transform = glm::rotate(transform, glm::radians(m_rotation_angle.x), {1,0,0}); 
    //transform = glm::rotate(transform, glm::radians(m_rotation_angle.y), {0,1,0}); 
    //transform = glm::rotate(transform, glm::radians(m_rotation_angle.z), {0,0,1}); 
    //transform = glm::translate(transform, m_position); 

    // inverting the transform matrix  
    //m_view_mat = glm::inverse(transform); 
    m_view_mat = glm::lookAt(m_position, m_position + m_front_vector, m_up_vector);
    m_view_projection_mat = m_projection_mat * m_view_mat; 
}

void pyro::perspective_camera::update_camera_vectors()
{
    // Calculate the new Front vector
    glm::vec3 front(0.f);
    float yaw_radians = glm::radians(m_yaw);
    float pitch_radians = glm::radians(m_pitch);
    front.x = cos(yaw_radians) * cos(pitch_radians);
    front.y = sin(pitch_radians);
    front.z = sin(yaw_radians) * cos(pitch_radians);

    m_front_vector = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_righ_vector = glm::normalize(glm::cross(m_front_vector, m_world_up_vector));  
    m_up_vector   = glm::normalize(glm::cross(m_righ_vector, m_front_vector));
    update_view_matrix();
}