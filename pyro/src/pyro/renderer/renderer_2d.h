#pragma once
#include "pyro/renderer/camera.h"
#include "pyro/renderer/texture_2d.h"

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

        static void begin_scene(camera &camera);
        static void end_scene();

        // primitives
        static void draw_quad(quad_properties const& props);
        static void draw_rotated_quad(quad_properties const& props);

    };
}
