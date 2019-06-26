#pragma once

#include "pyro/renderer/renderer_api.h"

namespace pyro
{
    /// \brief 
    class gl_render_api final : public renderer_api
    {
    public:
        void clear() override;
        void clear_color(const glm::vec4& color) override;

        void draw_indexed(const std::shared_ptr<vertex_array>& vertex_array) override;
    };
}
