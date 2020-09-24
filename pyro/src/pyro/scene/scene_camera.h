#pragma once
#include "pyro/renderer/camera.h"

namespace pyro
{
	// It represents all the real-time cameras present in the game.
	class scene_camera : public camera
	{
	public:
		enum class e_projection_type { perspective = 0, orthographic = 1 };

	public:
		scene_camera();
		virtual ~scene_camera() = default;

		void make_orthographic(float size, float nearClip, float farClip);
		void make_perspective(float vertical_fov, float near_clip, float far_clip);

		void viewport_size(uint32_t width, uint32_t height);

		float perspective_vertical_fov() const { return m_perspective_vertical_fov; }
		void  perspective_vertical_fov(float vertical_radians) { m_perspective_vertical_fov = vertical_radians; recalculate_projection(); }

		float perspective_near() const { return m_perspective_near; }
		void  perspective_near(float near_clip) { m_perspective_near = near_clip; recalculate_projection(); }

		float perspective_far() const { return m_perspective_far; }
		void  perspective_far(float far_clip) { m_perspective_far = far_clip; recalculate_projection(); }


		float orthographic_size() const { return m_orthographic_size; }
		void orthographic_size(float size) { m_orthographic_size = size; recalculate_projection(); }
		
		float orthographic_near() const { return m_orthographic_near; }
		void  orthographic_near(float near_clip) { m_orthographic_near = near_clip; recalculate_projection(); }
		
		float orthographic_far() const { return m_orthographic_far; }
		void  orthographic_far(float far_clip) { m_orthographic_far = far_clip; recalculate_projection(); }

		e_projection_type projection_type() const { return m_projection_type; }
		void projection_type(e_projection_type const &t) { m_projection_type = t; }

	private:
		void recalculate_projection();
	private:
		e_projection_type m_projection_type{ e_projection_type::orthographic };
		// Field of view angle stored in radians.
		float m_perspective_vertical_fov = glm::radians(45.f);
		float m_perspective_near = 0.1f;
		float m_perspective_far = 1000.f;

		// Full width of the orthographic camera fov.
		float m_orthographic_size = 10.0f;
		float m_orthographic_near = -1.0f;
		float m_orthographic_far = 1.0f;

		float m_aspect_ratio = 0.0f;
	};
}