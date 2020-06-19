#pragma once
#include "pyro/renderer/texture.h"

namespace pyro
{
    class PYRO_API frame_buffer
    {
    public:
        virtual ~frame_buffer() = default;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void clear() = 0;

        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual ref<texture> texture() const = 0;
    protected:
        virtual void init() = 0;
    };
}
