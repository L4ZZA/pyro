#pragma once
#include "renderer_api.h"

namespace pyro
{
    /// \brief
    class render_command
    {
    public:
        static void init()
        {
            s_renderer_api->init();
        }

        static void clear_color(const glm::vec4 &color)
        {
            s_renderer_api->clear_color(color);
        }

        static void clear()
        {
            s_renderer_api->clear();
        }

        static void toggle_wireframe()
        {
            static bool is_wireframe = true;
            if(is_wireframe)
                s_renderer_api->enable_wireframe();
            else
                s_renderer_api->disable_wireframe();

            is_wireframe = !is_wireframe;
        }
        
        static void submit(const ref<vertex_array>& vertex_array)
        {
            s_renderer_api->draw_indexed(vertex_array);
        }

    private:
        static renderer_api* s_renderer_api;
    };
}
