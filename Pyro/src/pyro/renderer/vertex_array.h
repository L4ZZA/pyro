#pragma once
#include "buffer.h"

namespace pyro
{
    class vertex_array
    {
    public:
        virtual ~vertex_array() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void add_buffer(const std::shared_ptr<vertex_buffer>& vertexBuffer) = 0;
        virtual void add_buffer(const std::shared_ptr<index_buffer>& indexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<vertex_buffer>>& vertex_buffers() const = 0;
        virtual const std::shared_ptr<index_buffer>& index_buffer() const = 0;

        static vertex_array* create();
    };
}
