#pragma once
#include <glm/glm.hpp>
#include "pyro/renderer/graphics_context.h"
#include "pyro/renderer/vertex_array.h"

namespace pyro
{
    enum class e_primitive_type;

    ///   
    class PYRO_API renderer_api
    {
    public:
        virtual ~renderer_api() = default;
        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void clear_color(glm::vec4 const &color) = 0;
        virtual void resize_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void enable_wireframe() = 0;
        virtual void disable_wireframe() = 0;

        virtual void draw_indexed(ref<vertex_array> const &vertex_array, const uint32_t index_count = 0) = 0;
        virtual void primitive_type(e_primitive_type const &type) = 0;
        static void api(e_renderer_api api) { s_renderer_api = api; }
        static e_renderer_api api() { return s_renderer_api; }

    protected:
        virtual void enable_alpha() = 0;
        virtual void enable_depth_test() = 0;
        virtual void enable_culling() = 0;
        static e_primitive_type s_primitive_type;
        inline static e_renderer_api s_renderer_api = e_renderer_api::opengl;
    };
}
