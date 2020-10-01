#pragma once
#include "pyro_pch.h"

#include "pyro/core/core.h"
#include "pyro/events/event.h"

namespace pyro
{
    struct PYRO_API window_props
    {
        std::string m_title       = "pyro";
        uint32_t    m_width       = 1280;
        uint32_t    m_height      = 720;
        bool        m_vsync       = false;
        bool        m_fullscreen  = false;
    };

    /// Interface representing a desktop system based window.
    class PYRO_API window
    {
    public:
        static scope<window> create(window_props const &props);

        //---------------------------------------------------------------------

        using event_callback_fn = std::function<void(event&)>;
        
        virtual ~window() = default;

        //---------------------------------------------------------------------

        virtual void on_update() = 0;

        //---------------------------------------------------------------------

        virtual void event_callback(event_callback_fn const &callback) = 0;
        virtual void vsync(bool enabled) = 0;
        virtual bool vsync() = 0;
        virtual void show_mouse_cursor() = 0;
        virtual void hide_mouse_cursor() = 0;
        virtual bool is_cursor_visible() const = 0;

        virtual void* native_window() const = 0;

        //---------------------------------------------------------------------

        virtual uint32_t width()  const = 0;
        virtual uint32_t height() const = 0;
    };
}