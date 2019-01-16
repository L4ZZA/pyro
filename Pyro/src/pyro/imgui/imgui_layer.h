#pragma once
#include "pyro/layer.h"

#include "pyro/events/application_event.h"
#include "pyro/events/mouse_event.h"
#include "pyro/events/key_event.h"

namespace pyro
{
    class PYRO_API imgui_layer : public pyro::layer
    {
    public:
        imgui_layer();
        ~imgui_layer();

        void on_attach() override;
        void on_detach() override;

        void on_update() override;
        void on_event(event& p_event) override;

        bool on_mouse_button_pressed_event(mouse_button_pressed_event &e);
        bool on_mouse_button_released_event(mouse_button_released_event &e);
        bool on_mouse_moved_event(mouse_moved_event &e);
        bool on_mouse_scrolled_event(mouse_scrolled_event &e);
        bool on_key_pressed_event(key_pressed_event &e);
        bool on_key_released_event(key_released_event &e);
        bool on_key_typed_event(key_typed_event &e);
        bool on_window_resize_event(window_resize_event &e);

    private:
        float m_time{ 0.f };
    };
}
