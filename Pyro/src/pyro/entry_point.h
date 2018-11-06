#pragma once

#ifdef PYRO_PLATFORM_WIN

#include <cstdio>

extern pyro::application* pyro::create_application();

int main(int argc, char** argv)
{
    printf("Pyro Engine win64");
    auto app = pyro::create_application();
    app->run();
    delete app;
}

#else
    #error Pyro only supports Windows!
#endif