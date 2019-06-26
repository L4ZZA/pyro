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
        virtual void clear() = 0;
        virtual void clear_color(const glm::vec4 &color) = 0;

        /// \brief 
        virtual void draw_indexed(const std::shared_ptr<vertex_array>& vertex_array) = 0;

        static e_api api() { return s_renderer_api; }
    
    private:
        inline static e_api s_renderer_api = e_api::open_gl;

    };
}
