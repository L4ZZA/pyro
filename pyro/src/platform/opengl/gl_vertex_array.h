#pragma once
#include "pyro/renderer/vertex_array.h"

namespace pyro
{
    class PYRO_API gl_vertex_array : public vertex_array
    {
    public:
        gl_vertex_array();
        ~gl_vertex_array();

        void bind() const override;
        void unbind() const override;

        void add_buffer(ref<vertex_buffer> const& vertex_buffer) override;
        void add_buffer(ref<pyro::index_buffer> const& index_buffer) override;

        std::vector<ref<vertex_buffer>> const& vertex_buffers() const override { return m_vertex_buffers; };
        ref<pyro::index_buffer> const& index_buffer() const override { return m_index_buffer; };

    private:
        uint32_t m_id = 0;
        uint32_t m_vertex_buffer_index = 0;
        std::vector<ref<vertex_buffer>> m_vertex_buffers;
        ref<pyro::index_buffer>         m_index_buffer;
    };
}
