#pragma once
#include "Event.h"
#include <sstream>


namespace pyro
{
    //=========================================================================

    class PYRO_API key_event : public event
    {
    public:
        int key_code() const { return key_code_; }
        EVENT_CLASS_CATEGORY(event_category_keyboard | event_category_input)
    protected:
        key_event(int p_keycode)
            : key_code_(p_keycode)
        {
        }
        int key_code_;
    };

    //=========================================================================

    class PYRO_API key_pressed_event : public key_event
    {
    public:
        key_pressed_event(int p_keycode, int repeatCount)
            : key_event(p_keycode), repeats_count_(repeatCount)
        {
        }
        int repeats_count() const { return repeats_count_; }
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "key_pressed_event: " << key_code_ << " (" << repeats_count_ << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(key_pressed)

    private:
        int repeats_count_;
    };

    //=========================================================================

    class PYRO_API key_released_event : public key_event
    {
    public:
        key_released_event(int p_keycode)
            : key_event(p_keycode)
        {
        }
        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "key_released_event: " << key_code_;
            return ss.str();
        }
        EVENT_CLASS_TYPE(key_released)
    };

    //=========================================================================
}
