#pragma once
#include "pyro/renderer/buffer.h"

namespace pyro
{
    //=================== vertex buffer =======================================

    /// \brief OpenGL specific implementation of the vertex_buffer interface
    class gl_vertex_buffer final : public vertex_buffer
    {
    public:
        gl_vertex_buffer(float* vertices, uint32_t size);
        ~gl_vertex_buffer();

        void bind() const override;
        void unbind() const override;

    private:
        uint32_t m_id{ 0 };
    };

    //=================== index buffer ========================================

    /// \brief OpenGL specific implementation of the index_buffer interface
    class gl_index_buffer final : public index_buffer
    {
    public:
        gl_index_buffer(uint32_t* vertices, uint32_t count);
        ~gl_index_buffer();

        void bind() const override;
        void unbind() const override;

        uint32_t count() const override;

    private:
        uint32_t m_id{ 0 };
        uint32_t m_count{ 0 };
    };
}
