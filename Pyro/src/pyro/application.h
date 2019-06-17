#pragma once

#include "core.h"

#include "window.h"
#include "layers_stack.h"
#include "events/event.h"
#include "events/application_event.h"
#include "imgui/imgui_layer.h"
#include "renderer/shader.h"
#include "renderer/buffer.h"
#include "renderer/vertex_array.h"


namespace pyro
{
    class PYRO_API application
    {
    public:
        application();
        virtual ~application();

        /// \brief
        void run();

        /// \brief
        void on_event(event &p_event);

        /// \brief Adds a layer to the stack.
        void push_layer(layer *p_layer);
        /// \brief Adds a overlay to the stack.
        void push_overlay(layer *p_overlay);

        /// \brief Returns a reference to the application window.
        window& get_window() const { return *m_window; }
        /// \brief Returns a reference to the application.
        static application& instance() { return *s_instance; }

    private:
        bool on_window_close(window_closed_event &p_event);

    private:
        std::unique_ptr<window>         m_window;
        bool                            m_running{ true };
        layers_stack                    m_layers_stack;

        std::shared_ptr<shader>         m_shader{};
        std::shared_ptr<vertex_array>   m_vertex_array{};

        std::shared_ptr<shader>         m_blue_shader{};
        std::shared_ptr<vertex_array>   m_rect_va{};

    private:
        static application*             s_instance;

    };


    application* create_application();
}
