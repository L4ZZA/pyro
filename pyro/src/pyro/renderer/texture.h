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
        none = 0, luminance, luminance_alpha, rgb, rgba, 
    };
    
    //-------------------------------------------------------------------------

    struct PYRO_API texture_parameters
    {
        e_texture_format format;
        e_texture_filter filter;
        e_texture_wrap   wrap;

        texture_parameters();
        texture_parameters(e_texture_filter filter);
        texture_parameters(e_texture_filter filter, e_texture_wrap wrap);
        texture_parameters(e_texture_format format, e_texture_filter filter, e_texture_wrap wrap);
    };

    //-------------------------------------------------------------------------

    class PYRO_API texture
    {
    public:
        virtual ~texture() = default;

        virtual uint32_t id() const = 0;
        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual std::string path() const = 0;

        virtual void data(void *data, uint32_t size) = 0;

        virtual void bind(uint32_t slot = 0) const = 0;

        virtual bool operator==(texture const &other) const = 0;
        virtual bool operator!=(texture const &other) const = 0;

    public:
        static void wrap(e_texture_wrap mode);
        static void filter(e_texture_filter mode);

    protected:
		static e_texture_wrap   s_wrap_mode;
		static e_texture_filter s_filter_mode;
    };
}
