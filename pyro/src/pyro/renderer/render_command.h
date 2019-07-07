#pragma once
#include "renderer_api.h"

namespace pyro
{
    /// \brief
    class render_command
    {
    public:
        static void clear_color(const glm::vec4 &color)
        {
            s_renderer_api->clear_color(color);
        }
        static void clear()
        {
            s_renderer_api->clear();
        }

        static void submit(const std::shared_ptr<vertex_array>& vertex_array)
        {
            s_renderer_api->draw_indexed(vertex_array);
        }

    private:
        static renderer_api* s_renderer_api;
    };
}
