#include "pyro_pch.h"
#include "scene_camera.h"

#include <glm/gtc/matrix_transform.hpp>


pyro::scene_camera::scene_camera()
{
	recalculate_projection();
}

void pyro::scene_camera::make_perspective(float vertical_fov, float near_clip, float far_clip)
{
	m_projection_type = e_projection_type::perspective;
	m_perspective_vertical_fov = vertical_fov;
	m_perspective_near = near_clip;
	m_perspective_far = far_clip;
	recalculate_projection();
}

void pyro::scene_camera::make_orthographic(float size, float near_clip, float far_clip)
{
	m_projection_type = e_projection_type::orthographic;
	m_orthographic_size = size;
	m_orthographic_near = near_clip;
	m_orthographic_far = far_clip;
	recalculate_projection();
}

void pyro::scene_camera::viewport_size(uint32_t width, uint32_t height)
{
	m_aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	recalculate_projection();
}

void pyro::scene_camera::recalculate_projection()
{
	if(m_projection_type == e_projection_type::perspective)
	{
		m_projection = glm::perspective(m_perspective_vertical_fov,
			m_aspect_ratio, m_perspective_near, m_perspective_far);
	}
	else
	{
		float half = 0.5f;
		
		float ortho_left   = -m_orthographic_size * m_aspect_ratio * half * m_orthographic_zoom_level;
		float ortho_right  =  m_orthographic_size * m_aspect_ratio * half * m_orthographic_zoom_level;
		float ortho_bottom = -m_orthographic_size * half * m_orthographic_zoom_level;
		float ortho_top    =  m_orthographic_size * half * m_orthographic_zoom_level;

		m_projection = glm::ortho(
			ortho_left, ortho_right,
			ortho_bottom, ortho_top, 
			m_orthographic_near, m_orthographic_far);
	}
}