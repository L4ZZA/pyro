#include "pyro_pch.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"



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
    glm::mat4 transform = glm::translate(glm::mat4{1}, m_position) * 
        glm::rotate(glm::mat4(1), glm::radians(m_rotation), glm::vec3(0,0,1));

    // inverting the transform matrix 
    m_view_mat = glm::inverse(transform);
    m_view_projection_mat = m_projection_mat * m_view_mat;
}
