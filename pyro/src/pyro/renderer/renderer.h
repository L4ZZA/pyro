#pragma once

#include "render_command.h"
#include "shader.h"
#include "camera.h"

namespace pyro
{
    class renderer
    {
    public:
        static void begin_scene(camera& camera);
        static void end_scene();

        static void submit(
            const std::shared_ptr<shader>& shader, 
            const std::shared_ptr<vertex_array>& vertex_array, 
            const glm::mat4& transform = glm::mat4(1.f));

        inline static renderer_api::e_api api() { return renderer_api::api(); }

    private:
        struct scene_data
        {
            glm::mat4 view_projection_matrix;
        };

        static scene_data* s_scene_data;
    };
}