#pragma once
#include "pyro_pch.h"

#include "pyro/core.h"
#include "pyro/events/event.h"

namespace pyro
{
    struct window_props
    {
        std::string     m_title;
        unsigned int    m_width;
        unsigned int    m_height;

        window_props(std::string const &p_title = "pyro",
                     unsigned int p_width = 1280,
                     unsigned int p_height = 720)
                : m_title(p_title), m_width(p_width), m_height(p_height)
        {
        }
    };

    /// \brief Interface representing a desktop system based window.
    class PYRO_API window
    {
    public:
        static window* create(window_props const &props = window_props());

        //---------------------------------------------------------------------

        using event_callback_fn = std::function<void(event&)>;
        
        virtual ~window() = default;

        //---------------------------------------------------------------------

        virtual void on_update() = 0;

        //---------------------------------------------------------------------

        virtual void event_callback(event_callback_fn const &p_callback) = 0;
        virtual void vsync(bool p_enabled) = 0;
        virtual bool vsync() = 0;


        //---------------------------------------------------------------------

        virtual unsigned int width()  const = 0;
        virtual unsigned int height() const = 0;

    };
}