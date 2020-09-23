#pragma once
#include "pyro/renderer/camera.h"

namespace pyro
{
	class scene_camera : public camera
	{
	public:
		scene_camera();
		virtual ~scene_camera() = default;

		void make_orthographic(float size, float nearClip, float farClip);

		void viewport_size(uint32_t width, uint32_t height);

		float orthographic_size() const { return m_orthographic_size; }
		void orthographic_size(float size) { m_orthographic_size = size; recalculate_projection(); }
	private:
		void recalculate_projection();
	private:
		float m_orthographic_size = 10.0f;
		float m_orthographic_near = -1.0f, m_orthographic_far = 1.0f;

		float m_aspect_ratio = 0.0f;
	};
}