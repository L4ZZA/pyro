#pragma once

namespace pyro
{
    enum class e_color_channels;
    enum class e_texture_filters;
    enum class e_wrap;

    class PYRO_API texture
    {
    public:
        virtual ~texture() = default;

        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual std::string path() const = 0;

        virtual void data(void *data, uint32_t size) = 0;

        virtual void bind(uint32_t slot = 0) const = 0;
    };

    class PYRO_API texture_2d : public texture
    {
    public:
        static ref<texture_2d> create(uint32_t width, uint32_t height);
        static ref<texture_2d> create(std::string const &path);
    };
}
