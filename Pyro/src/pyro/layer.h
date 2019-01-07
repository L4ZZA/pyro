#pragma once

#include "pyro/core.h"
#include "pyro/events/event.h"

namespace pyro
{

    class PYRO_API layer
    {
    public:
        layer(const std::string &p_name = "Layer");
        virtual ~layer();

        virtual void on_attach() {}
        virtual void on_detach() {}
        virtual void on_update() {}
        virtual void on_event(event &p_event) {}

        inline const std::string& name() const { return m_debug_name; }

    protected:
        std::string m_debug_name;
    };
}
