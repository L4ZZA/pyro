#pragma once
#include "pyro/renderer/graphics_context.h"

struct GLFWwindow;

namespace pyro::opengl
{
	class context : public graphics_context
	{
	public:
		context(GLFWwindow* window_handle);

		void init() override;
		void swap_buffers() override;

	private:
		GLFWwindow* m_window_handle;
	};
}
