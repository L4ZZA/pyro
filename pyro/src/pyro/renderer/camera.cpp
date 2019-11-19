#include "pyro_pch.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "pyro/core/key_codes.h"
#include "pyro/core/input.h"
#include "pyro/core/application.h"


//================== Orthographic Camera [2D] =================================

pyro::orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
    : m_projection_mat(glm::ortho(left, right, bottom, top, -1.f, 1.f))
{
    m_view_projection_mat = m_projection_mat * m_view_mat;
}

void pyro::orthographic_camera::projection_matrix(float left, float right, float bottom, float top)
{
    m_projection_mat = glm::ortho(left, right, bottom, top, -1.f, 1.f);
    m_view_projection_mat = m_projection_mat * m_view_mat;
}

void pyro::orthographic_camera::projection_matrix(glm::mat4 const& mat)
{
    m_projection_mat = mat;
}

void pyro::orthographic_camera::view_matrix(glm::mat4 const& mat)
{
    m_view_mat = mat;
}

void pyro::orthographic_camera::update_view_matrix()
{
    glm::mat4 transform(1); // REMEMBER STR -> ROTATE, TRANSLATE, SCALE in reverse.
    transform = glm::translate(transform, m_position);
    transform = glm::rotate(transform, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

    // inverting the transform matrix 
    m_view_mat = glm::inverse(transform);
    m_view_projection_mat = m_projection_mat * m_view_mat;
}

//================== Perspective Camera [3D] ==================================

// Default camera values 
const float YAW = -90.0f;
const float PITCH = 0.0f;

pyro::perspective_camera::perspective_camera(
    float width, float height,
    float fov /*= 45.f*/,
    float near_z /*= 0.1f*/, float far_z /*= 100.f*/)
    : m_projection_mat(glm::perspective(glm::radians(fov), width / height, near_z, far_z)),
    m_view_mat(1),
    m_view_projection_mat(1),
    m_aspect_ratio(width / height),
    m_fov(fov),
    m_near_plane(near_z),
    m_far_plane(far_z)

{
    m_up_vector = glm::vec3{0.f,1.f,0.f};
    m_right_vector = glm::vec3{0.f};
    m_world_up_vector = glm::vec3{0.f,1.f,0.f};

    m_rotation = glm::vec3{PITCH, YAW, 0.f};
    m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_front_vector = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
    m_view_mat = glm::lookAt(m_position, m_position + m_front_vector, m_up_vector);

    m_view_projection_mat = m_projection_mat * m_view_mat;
    PYRO_CORE_TRACE("3d cam position: [{},{},{}]", m_position.x, m_position.y, m_position.z);
    PYRO_CORE_TRACE("3d cam rotation: [{},{},{}]", m_rotation.x, m_rotation.y, m_rotation.z);
}

void pyro::perspective_camera::projection_matrix(glm::mat4 const& mat)
{
    m_projection_mat = mat;
}

void pyro::perspective_camera::view_matrix(glm::mat4 const& mat)
{
    m_view_mat = mat;
}

const glm::mat4 &pyro::perspective_camera::projection_matrix() const
{
    return m_projection_mat;
}

const glm::mat4 &pyro::perspective_camera::view_matrix() const
{
    return m_view_mat;
}

const glm::mat4 &pyro::perspective_camera::view_projection_matrix() const
{
    return m_view_projection_mat;
}

void pyro::perspective_camera::update_view_matrix()
{
    m_view_mat = glm::lookAt(m_position, m_position + m_front_vector, m_up_vector);
    m_view_projection_mat = m_projection_mat * m_view_mat;
}

void pyro::perspective_camera::update_camera_vectors()
{
    // Calculate the new Front vector
    glm::vec3 front(0.f);
    const float &yaw = m_rotation.y;
    const float &pitch = m_rotation.x;
    float yaw_radians = glm::radians(yaw);
    float pitch_radians = glm::radians(pitch);
    front.x = cos(yaw_radians) * cos(pitch_radians);
    front.y = sin(pitch_radians);
    front.z = sin(yaw_radians) * cos(pitch_radians);

    m_front_vector = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_right_vector = glm::normalize(glm::cross(m_front_vector, m_world_up_vector));
    m_up_vector = glm::normalize(glm::cross(m_right_vector, m_front_vector));
    update_view_matrix();
}
