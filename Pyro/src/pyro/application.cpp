#include "pyro_pch.h"
#include "application.h"

#include "logger.h"
#include "events/application_event.h"

pyro::application::application()
{
    
}

pyro::application::~application()
{
    
}

void pyro::application::run()
{
    window_resize_event e(1280, 720);
    if (e.is_in_category(event_category_application))
    {
        PYRO_TRACE(e);
    }
    if (e.is_in_category(event_category_input))
    {
        PYRO_TRACE(e);
    }

    while (true)
    {
        
    }
}
