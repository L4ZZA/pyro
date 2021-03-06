#pragma once
#include "pyro_pch.h"
#include "pyro/core/core.h"


namespace  pyro
{

    //=========================================================================

    enum class e_event_type
    {
        none = 0,
        window_close, window_resize, window_focus, window_lost_focus, window_moved,
        app_tick, app_update, app_render,
        key_pressed, key_released, key_typed,
        mouse_button_pressed, mouse_button_released, mouse_moved, mouse_scrolled
    };

    //-------------------------------------------------------------------------

    enum e_event_category
    {
        none = 0,
        event_category_application  = BIT(0),
        event_category_input        = BIT(1),
        event_category_keyboard     = BIT(2),
        event_category_mouse        = BIT(3),
        event_category_mouse_button = BIT(4),
    };

    //-------------------------------------------------------------------------

#define EVENT_CLASS_TYPE(type) static e_event_type static_type() { return e_event_type::type; }\
                                virtual e_event_type event_type() const override { return static_type(); }\
                                virtual const char* name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int category_flags() const override { return category; }

    //=========================================================================

    /// 
    class event
    {

    public:
        bool handled{ false };

    public:
        virtual ~event() = default;
        virtual e_event_type event_type() const = 0;
        virtual const char* name() const = 0;
        virtual int category_flags() const = 0;
        virtual std::string to_string() const { return name(); }

        ///Tells if event is in the given category.
        ///\param category 
        ///\return 0 (false) if not in the category, true elsewhere
        bool is_in_category(e_event_category category) const
        {
            return category_flags() & category;
        }

    };

    //=========================================================================

    /// 
    class event_dispatcher
    {
        //---------------------------------------------------------------------
    public:
        event_dispatcher(event& event)
            : m_event(event)
        {
        }

        /// F will be deducted by the compiler
        template<typename T, typename F>
        bool dispatch(const F& func)
        {
            if(m_event.handled)
                return false;

            // filtering events by type T
            if (m_event.event_type() == T::static_type())
            {
                m_event.handled = func(*static_cast<T*>(&m_event));
                return true;
            }
            return false;
        }

        //---------------------------------------------------------------------
    private:
        event& m_event;
    };

    //=========================================================================

    inline std::ostream& operator<<(std::ostream& os, const event& e)
    {
        return os << e.to_string();
    }

    //=========================================================================

}