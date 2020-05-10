#pragma once

namespace pyro
{
    enum class PYRO_API e_texture_wrap
    {
        none = 0, repeat, mirrored_repeat, clamp_to_edge, clamp_to_border
    };
    enum class PYRO_API e_texture_filter
    {
        none = 0, linear, nearest
    };
    enum class PYRO_API e_texture_format
    {
        none = 0, red, rg, rgb, rgba, 
    };
    enum class PYRO_API e_texture_data_type
    {
        unsigned_byte = 0, Float,
    };
    
    //-------------------------------------------------------------------------

    struct PYRO_API texture_parameters
    {
        e_texture_format format = e_texture_format::rgba;
        e_texture_filter filter = e_texture_filter::linear;
        e_texture_wrap   wrap   = e_texture_wrap::clamp_to_edge;
    };

    //-------------------------------------------------------------------------


// from glad.h GL_UNSIGNED_BYTE 0x1401
#define UNSIGNED_BYTE 0x1401

    class PYRO_API texture
    {
    public:
        virtual ~texture() = default;

        virtual uint32_t id() const = 0;
        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual std::string path() const = 0;
        // Returns the number of bytes needed to store a pixel based on the texture format.
        virtual uint32_t bytes_per_pixel() const = 0;

        virtual void data(void *data, uint32_t size, e_texture_data_type type = e_texture_data_type::unsigned_byte) = 0;

        virtual void bind(uint32_t slot = 0) const = 0;

        virtual bool operator==(texture const &other) const = 0;
        virtual bool operator!=(texture const &other) const = 0;

    };
}
