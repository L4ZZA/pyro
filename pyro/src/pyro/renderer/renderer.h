#pragma once

#include "render_command.h"
#include "shader.h"
#include "camera.h"

namespace pyro
{
    class renderer
    {
    public:
        static void init();
        static void shutdown();

        static void begin_scene(camera const &camera, const ref<shader> &shader);
        static void end_scene();
        static void on_window_resize(uint32_t width, uint32_t height);

        static void submit(
            const ref<shader> &shader,
            const ref<vertex_array> &vertex_array,
            const glm::mat4 &transform = glm::mat4(1.f));

        inline static renderer_api::e_api api() { return renderer_api::api(); }

    private:
        struct scene_data
        {
            glm::mat4 view_projection_matrix;
            ref<shader> shader;
        };

        static scene_data *s_scene_data;
    };
}