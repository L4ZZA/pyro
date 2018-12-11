#pragma once

#include "core.h"
#include "events/event.h"

namespace pyro
{
    class PYRO_API application
    {
    public:
        application();
        virtual ~application();

        void run();
    };


    application* create_application();
}
