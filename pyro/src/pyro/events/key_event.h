#pragma once
#include "event.h"


namespace pyro
{
    //=========================================================================

    class PYRO_API key_event : public event
    {
    public:
        int key_code() const { return m_key_code; }
        EVENT_CLASS_CATEGORY(event_category_keyboard | event_category_input)

    protected:
        key_event(int p_keycode)
            : m_key_code(p_keycode)
        {
        }

        int m_key_code;
    };

    //=========================================================================

    class PYRO_API key_pressed_event : public key_event
    {
    public:
        key_pressed_event(int p_keycode, int repeatCount)
            : key_event(p_keycode), m_repeats_count(repeatCount)
        {
        }
        
        int repeats_count() const { return m_repeats_count; }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "key_pressed_event: " << m_key_code << " (" << m_repeats_count << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(key_pressed)

    private:
        int m_repeats_count;
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
            ss << "key_released_event: " << m_key_code;
            return ss.str();
        }

        EVENT_CLASS_TYPE(key_released)
    };

    //=========================================================================

    /// \brief Represents the char value of the key pressed. 
    /// [Most commonly known as char_event in windows]
    class PYRO_API key_typed_event : public key_event
    {
    public:
        key_typed_event(int p_keycode)
            : key_event(p_keycode)
        {
        }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << "key_typed_event: " << m_key_code;
            return ss.str();
        }

        EVENT_CLASS_TYPE(key_typed)
    };

    //=========================================================================
}
