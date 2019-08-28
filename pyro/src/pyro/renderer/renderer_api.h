#pragma once
#include <glm/glm.hpp>
#include "vertex_array.h"

namespace pyro
{
    class renderer_api
    {
    public:
        enum class e_api
        {
            none = 0, open_gl = 1
        };

    public:
        virtual ~renderer_api() = default;
        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void clear_color(const glm::vec4 &color) = 0;

        /// \brief 
        virtual void draw_indexed(const ref<vertex_array>& vertex_array) = 0;
        virtual void enable_wireframe() = 0;
        virtual void disable_wireframe() = 0;

        static e_api api() { return s_renderer_api; }
    
    protected:
        virtual void enable_alpha() = 0;
        virtual void enable_culling() = 0;

    private:
        inline static e_api s_renderer_api = e_api::open_gl;

    };
}
