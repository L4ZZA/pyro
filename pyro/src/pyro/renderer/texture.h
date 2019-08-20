#pragma once

namespace pyro
{
    class texture
    {
    public:
        virtual ~texture() = default;

        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;

        virtual void bind(uint32_t slot = 0) const = 0;
    };

    class texture_2d : public texture
    {
    public:
        static ref<texture_2d> create(const std::string& path);
    };
}
