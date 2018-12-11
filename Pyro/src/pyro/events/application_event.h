#pragma once
#include "event.h"
#include <sstream>

namespace pyro
{
    //=========================================================================

    class PYRO_API window_resize_event : public event
    {
    public:
        window_resize_event(unsigned int width, unsigned int height)
            : width_(width), height_(height)
        {
        }
        unsigned int GetWidth() const { return width_; }
        unsigned int GetHeight() const { return height_; }
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "window_resize_event: " << width_ << ", " << height_;
            return ss.str();
        }
        EVENT_CLASS_TYPE(window_resize)
        EVENT_CLASS_CATEGORY(event_category_application)
    private:
        unsigned int width_, height_;
    };

    //=========================================================================

    class PYRO_API window_closed_event : public event
    {
    public:
        window_closed_event() {}
        EVENT_CLASS_TYPE(window_close)
        EVENT_CLASS_CATEGORY(event_category_application)
    };

    //=========================================================================

    class PYRO_API app_tick_event : public event
    {
    public:
        app_tick_event() {}
        EVENT_CLASS_TYPE(app_tick)
        EVENT_CLASS_CATEGORY(event_category_application)
    };

    //=========================================================================

    class PYRO_API app_update_event : public event
    {
    public:
        app_update_event() {}
        EVENT_CLASS_TYPE(app_update)
        EVENT_CLASS_CATEGORY(event_category_application)
    };

    //=========================================================================

    class PYRO_API app_render_event : public event
    {
    public:
        app_render_event() {}
        EVENT_CLASS_TYPE(app_render)
        EVENT_CLASS_CATEGORY(event_category_application)
    };

    //=========================================================================
}