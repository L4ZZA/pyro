#pragma once

#include "core.h"

#include "window.h"
#include "layers_stack.h"
#include "pyro/events/event.h"
#include "pyro/events/application_event.h"
#include "pyro/imgui/imgui_layer.h"
#include "pyro/core/timestep.h"
#include "pyro/core/timer.h"

int main(int argc, char** argv);

namespace pyro
{
    class PYRO_API application
    {
    public:
        application(uint32_t width, uint32_t height);
        virtual ~application();

        virtual void on_event(event &e);

        /// Adds a layer to the stack.
        void push_layer(ref<layer> const &layer);
        /// Adds a overlay to the stack.
        void push_overlay(ref<layer> const &overlay);

        /// Returns a reference to the application window.
        static window &window() { return *(s_instance->m_window); }

        /// Returns a reference to the application.
        static application &instance() { return *s_instance; }

        static uint32_t fps() { return instance().m_FramesPerSecond; }
        static uint32_t ups() { return instance().m_UpdatesPerSecond; }
        // Frame time in seconds
        static float    frame_time() { return instance().m_frame_time; }
    public:
        static void exit();

    protected:
        virtual void init() = 0;
        virtual void deinit() = 0;

    private:
        void run();
        bool on_window_close(window_closed_event &e);
        bool on_window_resized(window_resize_event &e);

    private:
        std::unique_ptr<pyro::window>   m_window;
        layers_stack                    m_layers_stack;
        float                           m_frame_time = 0.f;
        pyro::timer                    *m_timer = nullptr;
        uint32_t                        m_UpdatesPerSecond;
        uint32_t                        m_FramesPerSecond;

    private:
        static application   *s_instance;
        static bool           s_running;
        static bool           s_minimized;

        friend int ::main(int argc, char** argv);
    };

    application *create_application();
}
