#pragma once

#include "core.h"
#include "events/event.h"
#include "window.h"

namespace pyro
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    class PYRO_API application
    {
    private:
        std::unique_ptr<window> window_;
        bool running_ = true;

    public:
        application();
        virtual ~application();

        void run();

        void on_event(event &p_event);
    };


    application* create_application();
}
