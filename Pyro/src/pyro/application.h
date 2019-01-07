#pragma once

#include "core.h"

#include "window.h"
#include "layers_stack.h"
#include "events/event.h"
#include "events/application_event.h"


namespace pyro
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    class PYRO_API application
    {
    public:
        application();
        virtual ~application();

        /// \brief
        void run();

        /// \brief
        void on_event(event &p_event);

        /// \brief Adds a layer to the stack.
        void push_layer(layer *p_layer);
        /// \brief Adds a overlay to the stack.
        void push_overlay(layer *p_overlay);

    private:
        bool on_window_close(window_closed_event &p_event);

    private:
        std::unique_ptr<window> m_window;
        bool                    m_running{ true };
        layers_stack            m_layers_stack;

    };


    application* create_application();
}
