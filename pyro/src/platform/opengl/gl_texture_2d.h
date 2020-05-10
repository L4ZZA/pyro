#pragma once
#include "pyro/renderer/texture_2d.h"

namespace pyro
{
    class PYRO_API gl_texture_2d : public texture_2d
    {
    public:
        gl_texture_2d(uint32_t width, uint32_t height, texture_parameters const &params);
        gl_texture_2d(std::string const &path, texture_parameters const &params);
        ~gl_texture_2d();
        void bind(uint32_t slot = 0) const override;
        void data(void *data, uint32_t size, e_texture_data_type type = e_texture_data_type::unsigned_byte) override;

        uint32_t id() const override { return m_id; }
        uint32_t width() const override { return m_width; }
        uint32_t height() const override { return m_height; }
        std::string path() const override { return m_path; }
        uint32_t bytes_per_pixel() const override;
        // Inherited via texture_2d
        virtual bool operator==(texture const& other) const override;
        virtual bool operator!=(texture const& other) const override;

    private:
        std::string m_path;
        uint32_t m_id;
        uint32_t m_width;
        uint32_t m_height;
        e_texture_format m_format;
        e_texture_filter m_filter;
        e_texture_wrap m_wrap;


    private:
        // returns the opengl equivalent of the internal format 
        static uint32_t texture_internal_format_to_gl(e_texture_format format);
        // returns the opengl equivalent of the data format 
        static uint32_t texture_format_to_gl(e_texture_format format);
        // returns the opengl equivalent of the type of data uploaded to the gpu to fill the texture.
        static uint32_t texture_data_type_to_gl(e_texture_data_type type);
        static uint32_t texture_wrap_to_gl(e_texture_wrap mode);
        static uint32_t texture_filter_to_gl(e_texture_filter mode);
    };
}
