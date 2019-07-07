#pragma once

#include "render_command.h"

namespace pyro
{
    class renderer
    {
    public:
        static void begin_scene();
        static void end_scene();

        static void submit(const std::shared_ptr<vertex_array>& vertex_array);

        inline static renderer_api::e_api api() { return renderer_api::api(); }
    };
}