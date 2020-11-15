#pragma once
#include "pyro/core/layer.h"

namespace pyro
{
    class PYRO_API imgui_layer : public pyro::layer
    {
    public:
        imgui_layer(uint32_t width, uint32_t height, std::string const &name = "imgui_layer");
        ~imgui_layer() = default;

        void on_attach() override;
        void on_detach() override;
        void on_render() const override;
        virtual void on_imgui_render();
        void on_event(event& e) override;

        void begin() const;
        void end() const;

        void block_events(bool block) { m_block_events = block; }

        void dark_theme();

    private:
        float m_time{0.f};
        bool m_block_events = true;
    };
}
