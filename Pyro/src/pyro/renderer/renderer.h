#pragma once

namespace pyro
{
    enum class e_renderer_api
    {
        none = 0, open_gl = 1
    };

    class renderer
    {
    public:
        static e_renderer_api api() { return s_renderer_api; }

    private:
        static inline e_renderer_api s_renderer_api = e_renderer_api::open_gl;
    };
}