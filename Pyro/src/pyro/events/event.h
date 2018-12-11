#pragma once
#include "pyro/core.h"
#include <string>
#include <functional>


namespace  pyro
{

    //=========================================================================

    enum class event_type_e
    {
        none = 0,
        window_close, window_resize, window_focus, window_lost_focus, window_moved,
        app_tick, app_update, app_render,
        key_pressed, key_released,
        mouse_button_pressed, mouse_button_released, mouse_moved, mouse_scrolled
    };

    //-------------------------------------------------------------------------

    enum event_category_e
    {
        none = 0,
        event_category_application = BIT(0),
        event_category_input = BIT(1),
        event_category_keyboard = BIT(2),
        event_category_mouse = BIT(3),
        event_category_mouse_button = BIT(4),
    };

    //-------------------------------------------------------------------------

#define EVENT_CLASS_TYPE(type) static event_type_e static_type() { return event_type_e::##type; }\
                                virtual event_type_e event_type() const override { return static_type(); }\
                                virtual const char* name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int category_flags() const override { return category; }

    //=========================================================================

    /// \brief 
    class event
    {
        friend class event_dispatcher;

        //---------------------------------------------------------------------
    public:
        virtual ~event() = default;
        virtual event_type_e event_type() const = 0;
        virtual const char* name() const = 0;
        virtual int category_flags() const = 0;
        virtual std::string to_string() const { return name(); }

        ///\brief Tells if event is in the given category.
        ///\param category 
        ///\return 0 (false) if not in the category, true elsewhere
        bool is_in_category(event_category_e category) const
        {
            return category_flags() & category;
        }

        //---------------------------------------------------------------------
    protected:
        bool handled_{ false };
    };

    //=========================================================================

    /// \brief 
    class event_dispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;

        //---------------------------------------------------------------------
    public:
        event_dispatcher(event& event)
            : event_(event)
        {
        }

        /// \brief 
        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (event_.event_type() == T::static_type())
            {
                event_.handled_ = func(*static_cast<T*>(&event_));
                return true;
            }
            return false;
        }

        //---------------------------------------------------------------------
    private:
        event& event_;
    };

    //=========================================================================

    inline std::ostream& operator<<(std::ostream& os, const event& e)
    {
        return os << e.to_string();
    }

    //=========================================================================

}