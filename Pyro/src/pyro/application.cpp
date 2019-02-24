#include "pyro_pch.h"
#include "application.h"
#include "glad/glad.h"
#include "pyro/input.h"

pyro::application* pyro::application::s_instance{ nullptr };

pyro::application::application()
{
    PYRO_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    m_window = std::unique_ptr<window>(window::create());
    m_window->event_callback(BIND_EVENT_FN(application::on_event));

    m_imgui_layer = new imgui_layer();
    push_overlay(m_imgui_layer);
}

pyro::application::~application()
{

}

void pyro::application::run()
{
    while (m_running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto* layer : m_layers_stack)
            layer->on_update();

        // this will be on the renderer thread.
        m_imgui_layer->begin();
        for (auto* layer : m_layers_stack)
            layer->on_imgui_render();
        m_imgui_layer->end();

        //auto[x, y] = input::mouse_position();
        //if (input::mouse_button_pressed(0) || input::mouse_button_pressed(1))
        //    PYRO_CORE_TRACE("{0}, {1}", x, y);

        m_window->on_update();
    }
}

void pyro::application::on_event(event& p_event)
{
    event_dispatcher dispatcher(p_event);
    dispatcher.dispatch<window_closed_event>(BIND_EVENT_FN(application::on_window_close));

    //PYRO_CORE_TRACE("{0}", p_event);

    // events are executed from top of the stack to bottom (aka end to start of the list)
    for (auto it = m_layers_stack.end(); it != m_layers_stack.begin(); )
    {
        (*--it)->on_event(p_event);
        // stop event propagation to next layer if flagged as handled
        if (p_event.handled)
            break;
    }
}

void pyro::application::push_layer(layer* p_layer)
{
    m_layers_stack.push_layer(p_layer);
    p_layer->on_attach();
}

void pyro::application::push_overlay(layer* p_overlay)
{
    m_layers_stack.push_overlay(p_overlay);
    p_overlay->on_attach();
}

bool pyro::application::on_window_close(window_closed_event& p_event)
{
    m_running = false;

    return true;
}
