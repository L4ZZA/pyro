#include "pyro_pch.h"
#include "application.h"
#include "GLFW/glfw3.h"

pyro::application::application()
{
    window_ = std::unique_ptr<window>(window::create());
    window_->event_callback(BIND_EVENT_FN(application::on_event));
}

pyro::application::~application()
{
    
}

void pyro::application::run()
{
    while (running_)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        window_->on_update();
    }
}

void pyro::application::on_event(event& p_event)
{   
    event_dispatcher dispatcher(p_event);
    dispatcher.dispatch<window_closed_event>(BIND_EVENT_FN(application::on_window_close));

    PYRO_CORE_TRACE("{0}", p_event);
}

bool pyro::application::on_window_close(window_closed_event& p_event)
{
    running_ = false;

    return true;
}
