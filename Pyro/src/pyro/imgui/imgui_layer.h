#pragma once
#include "pyro/layer.h"

namespace pyro
{
    class imgui_layer : public pyro::layer
    {
    public:
        imgui_layer();
        ~imgui_layer();

        void on_update() override;
        void on_event(event& p_event) override;

    private:


    };
}
