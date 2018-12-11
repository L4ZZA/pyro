#pragma once
#include "event.h"

namespace pyro
{
    //=============================================================================

    class PYRO_API mouse_moved_event : public event
    {
    public:
        mouse_moved_event(float p_x, float p_y)
            : mouse_x(p_x), mouse_y_(p_y)
        {
        }
        float x() const { return mouse_x; }
        float y() const { return mouse_y_; }
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "mouse_moved_event: " << mouse_x << ", " << mouse_y_;
            return ss.str();
        }
        EVENT_CLASS_TYPE(mouse_moved)

        EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)
    private:
        float mouse_x, mouse_y_;
    };

    //=============================================================================

    class PYRO_API mouse_scrolled_event : public event
    {
    public:
        mouse_scrolled_event(float p_x_offset, float p_y_offset)
            : x_offset(p_x_offset), y_offset(p_y_offset)
        {
        }
        float xoffset() const { return x_offset; }
        float yoffset() const { return y_offset; }
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "mouse_scrolled_event: " << xoffset() << ", " << yoffset();
            return ss.str();
        }
        EVENT_CLASS_TYPE(mouse_scrolled)
        EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)
    private:
        float x_offset, y_offset;
    };

    //=============================================================================

    class PYRO_API mouse_button_event : public event
    {
    public:
        int mouse_button() const { return button_; }
        EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)
    protected:
        mouse_button_event(int button)
            : button_(button)
        {
        }
        int button_;
    };

    //=============================================================================

    class PYRO_API mouse_button_pressed_event : public mouse_button_event
    {
    public:
        mouse_button_pressed_event(int button)
            : mouse_button_event(button)
        {
        }
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "mouse_button_pressed_event: " << button_;
            return ss.str();
        }
        EVENT_CLASS_TYPE(mouse_button_pressed)
    };

    //=============================================================================

    class PYRO_API mouse_button_release_event : public mouse_button_event
    {
    public:
        mouse_button_release_event(int button)
            : mouse_button_event(button)
        {
        }
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "mouse_button_release_event: " << button_;
            return ss.str();
        }
        EVENT_CLASS_TYPE(mouse_button_released)
    };

    //=============================================================================
}
