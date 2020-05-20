#pragma once
#include "pyro/renderer/shader.h"
#include "pyro/renderer/camera.h"
#include "pyro/renderer/texture.h"

namespace pyro
{
    struct PYRO_API quad_properties
    {
        glm::vec4 color         = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec3 position      = {0.f,0.f,0.f};
        glm::vec2 size          = {1.f,1.f};
        ref<texture_2d> texture = nullptr;
        float rotation          = 0.f;
        float tiling_factor     = 1.f;
    };

    class PYRO_API renderer_2d
    {
    public:
        static void init();
        static void shutdown();

        static void begin_scene(ref<camera> camera);
        static void end_scene();
        static void flush();

        static ref<shader> current_shader();

        // primitives
        static void draw_quad(quad_properties const& props);

        static const uint32_t s_quad_vertex_count = 4;
        static const uint32_t s_quad_indices = 6;
        static const uint32_t s_max_quads = 20000;
        static const uint32_t s_max_vertices = s_max_quads * s_quad_vertex_count;
        static const uint32_t s_max_indices = s_max_quads * s_quad_indices;
        static const uint32_t s_max_texture_slots = 32;
            
        struct statistics {

            uint32_t draw_calls = 0;
            uint32_t quad_count = 0;

            uint32_t total_vertex_count() const { return quad_count * s_quad_vertex_count; }
            uint32_t total_index_count() const { return quad_count * s_quad_indices; }
        };

        static void reset_stats();
        static statistics stats();

    private:
        static void reset_render_data();
		static void flush_and_reset();
	};
}
