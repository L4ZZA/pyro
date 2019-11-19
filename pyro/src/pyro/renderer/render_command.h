#pragma once
#include "renderer_api.h"
#include "renderer_2d.h"

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
 
        static void resize_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
        { 
            s_renderer_api->resize_viewport(x,y,width, height); 
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

        static void primitive_type(const e_primitive_type& type)
        {
            s_renderer_api->primitive_type(type);
        }

        static void draw_indexed(const ref<vertex_array>& vertex_array)
        {
            s_renderer_api->draw_indexed(vertex_array);
        }

    private:
        static renderer_api* s_renderer_api;
    };
}
