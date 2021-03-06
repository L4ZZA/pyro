#pragma once
#include "pyro/renderer/graphics_context.h"

struct GLFWwindow;

namespace pyro
{
    /// OpenGL specific implementation of the graphics_context interface
    class PYRO_API gl_context : public graphics_context
    {
    public:
        gl_context(GLFWwindow* window_handle);

        void init() override;
        void swap_buffers() override;

    private:
        GLFWwindow* m_window_handle;
    };

}