#include "pyro_pch.h"
#include "platform/opengl/gl_context.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

pyro::gl_context::gl_context(GLFWwindow* window_handle)
	: m_window_handle(window_handle)
{
	PYRO_CORE_ASSERT(m_window_handle, "[gl_context] window handle is null!");
}

void pyro::gl_context::init()
{
	PYRO_PROFILE_FUNCTION();
	glfwMakeContextCurrent(m_window_handle);
	const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	PYRO_CORE_ASSERT(status, "Could not load Glad!");

	PYRO_CORE_INFO("OpenGL Info:");
	PYRO_CORE_INFO("\tVendor: {}",   glGetString(GL_VENDOR));
	PYRO_CORE_INFO("\tRenderer: {}", glGetString(GL_RENDERER));
	PYRO_CORE_INFO("\tVersion: {}",  glGetString(GL_VERSION));
    
	#ifdef PYRO_ENABLE_ASSERTS
		int version_major;
		int version_minor;
		glGetIntegerv(GL_MAJOR_VERSION, &version_major);
		glGetIntegerv(GL_MINOR_VERSION, &version_minor);

		PYRO_CORE_ASSERT(version_major > 4 || (version_major == 4 && version_minor >= 5), "Pyro requires at least OpenGL version 4.5!");
	#endif
}

void pyro::gl_context::swap_buffers()
{
	PYRO_PROFILE_FUNCTION();
	glfwSwapBuffers(m_window_handle);
}
