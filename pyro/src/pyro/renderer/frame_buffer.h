#pragma once
#include "pyro/renderer/texture.h"
#include <glm/glm.hpp>

namespace pyro
{
    struct PYRO_API framebuffer_props
    {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t samples = 1;
        glm::vec4 clear_color = { 9.f, 0.f, 0.f, 1.f }; // { 1.f, 1.f, 1.f, 1.f };
        bool swap_chain_target = false;
    };

    class PYRO_API frame_buffer
    {
    public:
        virtual ~frame_buffer() = default;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void clear() = 0;
        virtual void resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual ref<texture_2d> const& color_attachment() const = 0;
        virtual uint32_t depth_attachment() const = 0;
    protected:
        virtual void init() = 0;
    };
}
