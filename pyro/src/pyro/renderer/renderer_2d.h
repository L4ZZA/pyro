#pragma once
#include "pyro/renderer/camera.h"
#include "pyro/renderer/texture.h"

namespace pyro
{
    class renderer_2d
    {
    public:
        static void init();
        static void shutdonw();

        static void begin_scene(camera &camera);
        static void end_scene();

        // primitives
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color);
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const ref<texture_2d>& texture);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const ref<texture_2d>& texture);
        
    };
}
