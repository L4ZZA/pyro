#include "pyro_pch.h"
#include "application.h"
#include "GLFW/glfw3.h"

pyro::application::application()
{
    window_ = std::unique_ptr<window>(window::create());
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
