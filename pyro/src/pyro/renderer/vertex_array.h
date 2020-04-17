#pragma once
#include "buffer.h"

namespace pyro
{
    class PYRO_API vertex_array
    {
    public:
        virtual ~vertex_array() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void add_buffer(ref<vertex_buffer> const& vertexBuffer) = 0;
        virtual void add_buffer(ref<index_buffer> const& indexBuffer) = 0;

        virtual std::vector<ref<vertex_buffer>> const& vertex_buffers() const = 0;
        virtual ref<index_buffer> const& index_buffer() const = 0;

        static ref<vertex_array> create();
    };
}
