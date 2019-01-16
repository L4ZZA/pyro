#pragma once

#ifdef PYRO_PLATFORM_WIN

extern pyro::application* pyro::create_application();

int main(int argc, char** argv)
{
    pyro::logger::init();
    PYRO_CORE_WARN("Initialized logger");
    PYRO_INFO("Hello!");

    auto app = pyro::create_application();
    app->run();
    delete app;
}

#else
    #error Pyro only supports Windows!
#endif