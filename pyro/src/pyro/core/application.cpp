#include "pyro_pch.h"
#include "application.h"

#include "platform/opengl/gl_shader.h"
#include "pyro/renderer/renderer.h"
#include "pyro/core/timer.h"
#include "GLFW/glfw3.h"

//----------------------------------------------------------------------------- 

pyro::application *pyro::application::s_instance = nullptr;

//----------------------------------------------------------------------------- 

pyro::application::application(const window_props &properties, e_renderer_api const &api /*= e_renderer_api::opengl*/)
{
    PYRO_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    renderer_api::api(api);
    m_window = window::create(properties);
    m_window->event_callback(BIND_EVENT_FN(application::on_event));

    renderer::init();
}

pyro::application::~application()
{
    renderer::shutdown();
}

void pyro::application::run()
{
    m_timer = new timer();
    float tot_time = 0.f;
    uint32_t frames = 0;
    uint32_t updates = 0;

    while(m_running)
    {
        PYRO_PROFILE_SCOPE("run loop");
        timestep timestep(m_timer->elapsed());
        tot_time += timestep;
        
        if(!m_suspended)
        {
            // Total time spent for all layers to be rendered
            float tot_frame_time = 0.f;
            // Total time spent for all layers to be updated
            float tot_update_time = 0.f;

            for(auto &layer : m_layers_stack)
            {
                {
                    timer update_time;
                    layer->on_update(timestep);
                    tot_update_time += update_time.elapsed();
                    updates++;
                }
                {
                    timer frame_time;
                    layer->on_render();
                    m_frame_time = frame_time.elapsed();
                    tot_frame_time += m_frame_time;
                    frames++;
                }
                if(layer->is_imgui() && layer == m_imgui_layer)
                {
                    PYRO_CORE_ASSERT(m_imgui_layer, "imgui_layer couldn't be cast!");
                    m_imgui_layer->begin();
                    {
                        m_imgui_layer->on_imgui_render();
                    }
                    m_imgui_layer->end();
                }
            }
            tot_time += tot_frame_time;
            tot_time += tot_update_time;
        }

        m_window->on_update();

        if(tot_time > 1.0f)
        {
            m_FramesPerSecond = frames;
            m_UpdatesPerSecond = updates;
            frames = 0;
            updates = 0;
            tot_time -= 1.0f;
        }
    }
}

void pyro::application::on_event(event &e)
{
    

    event_dispatcher dispatcher(e);
    // dispatch event on window X pressed 
    dispatcher.dispatch<window_closed_event>(BIND_EVENT_FN(application::on_window_close));
    // dispatch event on window resized
    dispatcher.dispatch<window_resize_event>(BIND_EVENT_FN(application::on_window_resized));

    //PYRO_CORE_TRACE("{0}", event); 

    // events are executed from top of the stack to bottom (aka end to start of the list) 
    for(auto it = m_layers_stack.end(); it != m_layers_stack.begin(); )
    {
        // stop event propagation to next layer if flagged as handled 
        if(e.handled)
            break;
        (*--it)->on_event(e);
    }
}

void pyro::application::push_layer(ref<layer> const &layer)
{
    m_layers_stack.push_layer(layer);
    layer->on_attach();
}

void pyro::application::push_overlay(ref<layer> const &overlay)
{
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
    

    if(e.height() == 0 || e.width() == 0)
    {
        m_suspended = true;
        return false;
    }
    m_suspended = false;

    renderer::on_window_resize(e.width(), e.height());

    const bool event_handled = false;
    return event_handled;
}

void pyro::application::start()
{
    init();
    m_running = true;
    m_suspended = false;
    run();
}

void pyro::application::suspend()
{
    m_suspended = true;
}

void pyro::application::resume()
{
    m_suspended = false;
}

void pyro::application::stop()
{
    m_running = false;
}

void pyro::application::exit()
{
    instance().stop();
}