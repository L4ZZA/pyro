#pragma once
#include "pyro/layer.h"

namespace pyro
{
    class PYRO_API imgui_layer : public pyro::layer
    {
    public:
        imgui_layer();
        ~imgui_layer() = default;

        void on_attach() override;
        void on_detach() override;
        void on_imgui_render() override;

        void begin() const;
        void end() const;

    private:
        float m_time{ 0.f };
    };
}
