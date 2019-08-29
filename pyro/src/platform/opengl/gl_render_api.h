#pragma once

#include "pyro/renderer/renderer_api.h"

namespace pyro
{
    /// \brief 
    class gl_render_api final : public renderer_api
    {
    public:
        void init() override;
        void clear() override;
        void clear_color(const glm::vec4& color) override;

        void enable_wireframe() override;
        void disable_wireframe() override;

        void draw_indexed(const ref<vertex_array>& vertex_array) override;

    private:
        void enable_alpha() override;
        void enable_culling() override;
    };
}
