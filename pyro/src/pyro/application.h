#pragma once

#include "core.h"

#include "window.h"
#include "layers_stack.h"
#include "events/event.h"
#include "events/application_event.h"
#include "imgui/imgui_layer.h"
#include "pyro/core/timestep.h"


namespace pyro
{
    class PYRO_API application
    {
    public:
        application();
        virtual ~application();

        /// \brief
        void run();

        /// \brief
        virtual void on_event(event &event);

        /// \brief Adds a layer to the stack.
        void push_layer(layer *layer);
        /// \brief Adds a overlay to the stack.
        void push_overlay(layer *overlay);

        /// \brief Returns a reference to the application window.
        window& get_window() const { return *m_window; }
        /// \brief Returns a reference to the application.
        static application& instance() { return *s_instance; }

    private:
        bool on_window_close(window_closed_event &event);

    private:
        std::unique_ptr<window>   m_window;
        layers_stack                    m_layers_stack;
        float                           m_last_frame_time = 0.f;

    private:
        static application*             s_instance;
        static bool                     s_running; 

    public: 
        static void exit(); 

    };


    application* create_application();
}
