#include "pyro_pch.h"
#include "platform/opengl/gl_context.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

pyro::gl_context::gl_context(GLFWwindow* window_handle)
	: m_window_handle(window_handle)
{
	PYRO_ASSERT(m_window_handle, "[gl_context] window handle is null!");
}

void pyro::gl_context::init()
{
	glfwMakeContextCurrent(m_window_handle);
	const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	PYRO_ASSERT(status, "Could not load Glad!");

	PYRO_CORE_INFO("OpenGL Info:");
	PYRO_CORE_INFO("\tVendor: {}", glGetString(GL_VENDOR));
	PYRO_CORE_INFO("\tRenderer: {}", glGetString(GL_RENDERER));
	PYRO_CORE_INFO("\tVersion: {}", glGetString(GL_VERSION));
}

void pyro::gl_context::swap_buffers()
{
	glfwSwapBuffers(m_window_handle);
}
