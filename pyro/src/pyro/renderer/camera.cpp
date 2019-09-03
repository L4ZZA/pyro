#include "pyro_pch.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"


//================== Orthographic Camera [2D] =================================

pyro::orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
    : m_projection_mat(glm::ortho(left, right, bottom, top, -1.f, 1.f))
{
    m_view_projection_mat = m_projection_mat * m_view_mat;
}

void pyro::orthographic_camera::move(e_direction direction, timestep ts)
{
    float speed = s_movement_speed;

    if (direction == up)
        m_position.y += speed * ts;
    else if(direction == down)
        m_position.y -= speed * ts;

    if (direction == left)
        m_position.x -= speed * ts;
    else if(direction == right)
        m_position.x += speed * ts;

    update_view_matrix();
}

void pyro::orthographic_camera::rotate(e_rotation rotation, timestep ts)
{
    float speed = s_rotation_speed;
    
    if (rotation == clock_wise)
    {
        m_rotation -= speed * ts;
    }
    else if (rotation == anticlock_wise)
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
    float width, float height, 
    const glm::vec3& start_position /*= {0.f,0.f,3.f}*/, 
    float fov /*= 45.f*/, 
    float near_z /*= 0.1f*/, float far_z /*= 100.f*/) 
    : m_projection_mat(glm::perspective(glm::radians(fov), width / height, near_z, far_z)), 
    m_position(start_position), 
    m_aspect_ratio(width / height), 
    m_fov(fov), 
    m_near_cplane(near_z), 
    m_far_cplane(far_z) 
{ 
    // note that we're translating the scene in the reverse direction of where we want to move 
    m_view_mat = translate(m_view_mat, m_position * -1.f); 
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

void pyro::perspective_camera::move(e_direction direction, timestep ts) 
{ 
    if(direction == front) 
        m_position.z += s_movement_speed * ts; 
    else if(direction == back) 
        m_position.z -= s_movement_speed * ts; 

    if(direction == left) 
        m_position.x -= s_movement_speed * ts; 
    else if(direction == right) 
        m_position.x += s_movement_speed * ts; 

    PYRO_CORE_TRACE("3d cam position: [{},{},{}]", m_position.x, m_position.y, m_position.z); 

    update_view_matrix(); 
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

    PYRO_CORE_TRACE("after - 3d cam rotation: [{},{},{}]", m_rotation_angle.x, m_rotation_angle.y, m_rotation_angle.z); 

    update_view_matrix(); 
} 

void pyro::perspective_camera::update_view_matrix() 
{ 
    glm::mat4 transform{1}; 
    transform = glm::rotate(transform, glm::radians(m_rotation_angle.x), {1,0,0}); 
    transform = glm::rotate(transform, glm::radians(m_rotation_angle.y), {0,1,0}); 
    transform = glm::rotate(transform, glm::radians(m_rotation_angle.z), {0,0,1}); 
    transform = glm::translate(transform, m_position); 

    // inverting the transform matrix  
    m_view_mat = glm::inverse(transform); 
    m_view_projection_mat = m_projection_mat * m_view_mat; 
} 