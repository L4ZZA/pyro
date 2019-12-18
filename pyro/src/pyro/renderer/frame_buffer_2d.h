#pragma once
#include "pyro/renderer/frame_buffer.h"

namespace pyro
{
    class frame_buffer_2d : public frame_buffer
    {
    public:
        virtual void clear_color(glm::vec4 const &color) = 0;

    public:
        static ref<frame_buffer_2d> create(uint32_t width, uint32_t height);
    };
}
