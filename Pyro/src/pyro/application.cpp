#include "pyro_pch.h"
#include "application.h"

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
        window_->on_update();
    }
}
