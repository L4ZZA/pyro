#pragma once

#include "Core.h"

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
