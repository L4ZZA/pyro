#include "pyro_pch.h"
#include "platform/opengl/context.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

pyro::opengl_context::opengl_context(GLFWwindow* window_handle)
	: m_window_handle(window_handle)
{
	PYRO_ASSERT(m_window_handle, "[opengl_context] window handle is null!")
}

void pyro::opengl_context::init()
{
	glfwMakeContextCurrent(m_window_handle);
	const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	PYRO_ASSERT(status, "Could not load Glad!");
}

void pyro::opengl_context::swap_buffers()
{
	glfwSwapBuffers(m_window_handle);
}
