#pragma once

#include "core.h"
#include "events/event.h"
#include "window.h"

namespace pyro
{
    class PYRO_API application
    {
    private:
        std::unique_ptr<window> window_;
        bool running_ = true;

    public:
        application();
        virtual ~application();

        void run();
    };


    application* create_application();
}
