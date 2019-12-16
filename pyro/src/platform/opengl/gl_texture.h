#pragma once
#include "pyro/renderer/texture.h"

namespace pyro
{
    class gl_texture_2d : public texture_2d
    {
    public:
        gl_texture_2d(uint32_t width, uint32_t height);
        gl_texture_2d(std::string const &path);
        ~gl_texture_2d();
        void bind(uint32_t slot = 0) const override;
        void data(void *data, uint32_t size) override;

        uint32_t width() const override { return m_width; }
        uint32_t height() const override { return m_height; }
        std::string path() const override { return m_path; }

    private:
        std::string m_path;
        uint32_t m_width{0};
        uint32_t m_height{0};
        uint32_t m_id{0};
        uint32_t m_data_format{0};
        uint32_t m_internal_format{0};
    };
}
