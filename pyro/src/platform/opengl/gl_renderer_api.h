#pragma once
#include "pyro/renderer/renderer_api.h"

namespace pyro
{
    /// \brief 
    class PYRO_API gl_renderer_api final : public renderer_api
    {
    public:
        ~gl_renderer_api() = default;
        void init() override;

        void clear() override;
        void clear_color(const glm::vec4& color) override;
        void resize_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        void draw_indexed(const ref<vertex_array>& vertex_array, const uint32_t index_count = 0) override;

        void enable_wireframe() override;
        void disable_wireframe() override;
        void primitive_type(const e_primitive_type& type) override;

    protected:
        void enable_alpha() override;
        void enable_depth_test() override;
        void enable_culling() override;
    };
}
