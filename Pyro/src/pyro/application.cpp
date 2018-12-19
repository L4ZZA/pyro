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
    PYRO_CORE_INFO("{0}", p_event);
}
