#pragma once
#include "pyro/layer.h"

#include "pyro/events/application_event.h"
#include "pyro/events/mouse_event.h"
#include "pyro/events/key_event.h"

namespace pyro
{
    class PYRO_API imgui_layer : public pyro::layer
    {
    public:
        imgui_layer();
        ~imgui_layer();

        virtual void on_attach() override;
        virtual void on_detach() override;
        virtual void on_imgui_render() override;

        void begin() const;
        void end() const;

    private:
        float m_time{ 0.f };
    };
}
