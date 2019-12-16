#include "pyro_pch.h"
#include "application.h"

#include "platform/opengl/gl_shader.h"
#include "pyro/renderer/renderer.h"
#include "GLFW/glfw3.h"

//----------------------------------------------------------------------------- 

pyro::application *pyro::application::s_instance = nullptr;
bool pyro::application::s_running = true;
bool pyro::application::s_minimized = false;

//----------------------------------------------------------------------------- 

pyro::application::application()
{
    PYRO_PROFILE_FUNCTION();

    PYRO_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    m_window = std::unique_ptr<pyro::window>(window::create());
    m_window->event_callback(BIND_EVENT_FN(application::on_event));

    renderer::init();
}

pyro::application::~application()
{
    PYRO_PROFILE_FUNCTION();
    renderer::shutdown();
}

void pyro::application::run()
{
    PYRO_PROFILE_FUNCTION();
    while(s_running)
    {
        PYRO_PROFILE_SCOPE("run loop");
        float time = static_cast<float>(glfwGetTime()); //  platform independent
        timestep timestep = time - m_last_frame_time;
        m_last_frame_time = time;

        if(!s_minimized)
        {
            for(auto *layer : m_layers_stack)
            {
                {
                    PYRO_PROFILE_SCOPE("layer_stack - on_update");
                    layer->on_update(timestep);
                }

                if(layer->is_imgui())
                {
                    auto *const imgui_layer = dynamic_cast<pyro::imgui_layer *>(layer);
                    PYRO_CORE_ASSERT(imgui_layer, "imgui_layer couldn't be cast!");
                    imgui_layer->begin();
                    {
                        PYRO_PROFILE_SCOPE("layer_stack - on_imgui_render");
                        imgui_layer->on_imgui_render();
                    }
                    imgui_layer->end();
                }
            }
        }

        m_window->on_update();
    }
}

void pyro::application::on_event(event &e)
{
    PYRO_PROFILE_FUNCTION();

    event_dispatcher dispatcher(e);
    // dispatch event on window X pressed 
    dispatcher.dispatch<window_closed_event>(BIND_EVENT_FN(application::on_window_close));
    // dispatch event on window resized
    dispatcher.dispatch<window_resize_event>(BIND_EVENT_FN(application::on_window_resized));

    //PYRO_CORE_TRACE("{0}", event); 

    // events are executed from top of the stack to bottom (aka end to start of the list) 
    for(auto it = m_layers_stack.end(); it != m_layers_stack.begin(); )
    {
        (*--it)->on_event(e);
        // stop event propagation to next layer if flagged as handled 
        if(e.handled)
            break;
    }
}

void pyro::application::push_layer(layer *layer)
{
    PYRO_PROFILE_FUNCTION();

    m_layers_stack.push_layer(layer);
    layer->on_attach();
}

void pyro::application::push_overlay(layer *overlay)
{
    PYRO_PROFILE_FUNCTION();

    m_layers_stack.push_overlay(overlay);
    overlay->on_attach();
}

bool pyro::application::on_window_close(window_closed_event &)
{
    exit();
    const bool event_handled = false;
    return event_handled;
}

bool pyro::application::on_window_resized(window_resize_event &e)
{
    PYRO_PROFILE_FUNCTION();

    if(e.height() == 0 || e.width() == 0)
    {
        s_minimized = true;
        return false;
    }
    s_minimized = false;

    renderer::on_window_resize(e.width(), e.height());

    const bool event_handled = false;
    return event_handled;
}

void pyro::application::exit()
{
    s_running = false;
}