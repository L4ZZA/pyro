#pragma once

#include "core.h"
#include "events/event.h"
#include "events/application_event.h"

#include "window.h"

namespace pyro
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    class PYRO_API application
    {
    private:
        std::unique_ptr<window> m_window;
        bool m_running = true;

    public:
        application();
        virtual ~application();

        void run();

        void on_event(event &p_event);

    private:
        bool on_window_close(window_closed_event &p_event);
    };


    application* create_application();
}
