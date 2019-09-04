#include "pyro_pch.h"
#include "application.h"

#include "platform/opengl/gl_shader.h"
#include "renderer/renderer.h"
#include "GLFW/glfw3.h"

//----------------------------------------------------------------------------- 

pyro::application* pyro::application::s_instance{ nullptr }; 
bool pyro::application::s_running{ true }; 

//----------------------------------------------------------------------------- 

pyro::application::application()
{
    PYRO_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    m_window = std::unique_ptr<pyro::window>(window::create());
    m_window->event_callback(BIND_EVENT_FN(application::on_event));

    render_command::init();

}

pyro::application::~application()
{
}

void pyro::application::run()
{
    while (s_running)
    {
        float time = static_cast<float>(glfwGetTime()); //  platform independent
        timestep timestep = time - m_last_frame_time;
        m_last_frame_time = time;

        for (auto* layer : m_layers_stack)
        {
            layer->on_update(timestep);

            if (layer->is_imgui())
            {
                auto imgui_layer = dynamic_cast<pyro::imgui_layer*>(layer);
                assert(imgui_layer, "imgui_layer couldn't be cast!");
                imgui_layer->begin();
                imgui_layer->on_imgui_render();
                imgui_layer->end();
            }
        }

        m_window->on_update();
    }

}

void pyro::application::on_event(event& event) 
{ 
    event_dispatcher dispatcher(event); 
    // dispatch event on window X pressed 
    dispatcher.dispatch<window_closed_event>(BIND_EVENT_FN(application::on_window_close)); 

    //PYRO_CORE_TRACE("{0}", event); 

    // events are executed from top of the stack to bottom (aka end to start of the list) 
    for (auto it = m_layers_stack.end(); it != m_layers_stack.begin(); ) 
    { 
        (*--it)->on_event(event); 
        // stop event propagation to next layer if flagged as handled 
        if (event.handled) 
            break; 
    } 
} 

void pyro::application::push_layer(layer* layer) 
{ 
    m_layers_stack.push_layer(layer); 
} 

void pyro::application::push_overlay(layer* overlay) 
{ 
    m_layers_stack.push_overlay(overlay); 
} 

bool pyro::application::on_window_close(window_closed_event&) 
{ 
    exit(); 
    return true; 
} 

void pyro::application::exit() 
{ 
    s_running = false; 
} 