#pragma once
#include "pyro/renderer/frame_buffer_2d.h"
#include "pyro/renderer/texture.h"

namespace pyro
{
    class gl_frame_buffer_2d final : public frame_buffer_2d
    {
    public:
        gl_frame_buffer_2d(framebuffer_props const &properties);
        ~gl_frame_buffer_2d();

        void bind() const override;
        void unbind() const override;

        void resize(uint32_t width, uint32_t height) override;
        void clear_color(glm::vec4 const& color) override;
        void clear() override;

        uint32_t width() const override;
        uint32_t height() const override;
        uint32_t color_attachment() const override;
        uint32_t depth_attachment() const override;


    private:
        void init() override;

    private:
        uint32_t m_id;
        uint32_t m_depth_buffer_id;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_color_attachment;
        uint32_t m_depth_attachment;
        glm::vec4 m_clear_color;
    };
}
