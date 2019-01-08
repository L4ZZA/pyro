#pragma once
#include "pyro/layer.h"

namespace pyro
{
    class PYRO_API imgui_layer : public pyro::layer
    {
    public:
        imgui_layer();
        ~imgui_layer();

        void on_attach() override;
        void on_detach() override;

        void on_update() override;
        void on_event(event& p_event) override;

    private:
        float m_time{ 0.f };


    };
}
