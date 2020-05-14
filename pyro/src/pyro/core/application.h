#pragma once

#include "core.h"

#include "window.h"
#include "layers_stack.h"
#include "pyro/events/event.h"
#include "pyro/events/application_event.h"
#include "pyro/imgui/imgui_layer.h"
#include "pyro/core/timestep.h"

int main(int argc, char** argv);

namespace pyro
{
    class PYRO_API application
    {
    public:
        application(uint32_t width, uint32_t height);
        virtual ~application();

        /// \brief
        virtual void on_event(event &event);

        /// \brief Adds a layer to the stack.
        void push_layer(ref<layer> const &layer);
        /// \brief Adds a overlay to the stack.
        void push_overlay(ref<layer> const &overlay);

        /// \brief Returns a reference to the application window.
        static window &window() { return *(s_instance->m_window); }

        /// \brief Returns a reference to the application.
        static application &instance() { return *s_instance; }

    public:
        static void exit();

    private:
        /// \brief
        void run();
        virtual void init() = 0;
        virtual void deinit() = 0;
        bool on_window_close(window_closed_event &e);
        bool on_window_resized(window_resize_event &e);

    private:
        std::unique_ptr<pyro::window>   m_window;
        layers_stack                    m_layers_stack;
        float                           m_last_frame_time = 0.f;

    private:
        static application   *s_instance;
        static bool           s_running;
        static bool           s_minimized;

        friend int ::main(int argc, char** argv);
    };


    application *create_application();
}
