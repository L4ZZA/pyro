#pragma once

namespace pyro
{
    //=================== vertex buffer ===================================

    class vertex_buffer
    {
    public:
        virtual ~vertex_buffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        static vertex_buffer* create(float* vertices, uint32_t size);
    };

    //=================== index buffer ========================================

    class index_buffer
    {
    public:
        virtual ~index_buffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t count() const = 0;

        static index_buffer* create(uint32_t* indices, uint32_t count);
    };

}
