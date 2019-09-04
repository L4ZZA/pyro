#pragma once
#include "pyro/renderer/vertex_array.h"

namespace pyro
{
    class gl_vertex_array : public vertex_array
    {
    public:
        gl_vertex_array();
        ~gl_vertex_array();

        void bind() const override;
        void unbind() const override;

        void add_buffer(const ref<vertex_buffer>& vertex_buffer) override;
        void add_buffer(const ref<pyro::index_buffer>& index_buffer) override;

        const std::vector<ref<vertex_buffer>>& vertex_buffers() const override { return m_vertex_buffers; };
        const ref<pyro::index_buffer>& index_buffer() const override { return m_index_buffer; };

    private:
        uint32_t m_id = 0;
        uint32_t m_vertex_buffer_index = 0;
        std::vector<ref<vertex_buffer>> m_vertex_buffers;
        ref<pyro::index_buffer>         m_index_buffer;
    };
}
