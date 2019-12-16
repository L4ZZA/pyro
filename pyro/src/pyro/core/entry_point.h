#pragma once
#include "application.h"

#ifdef PYRO_PLATFORM_WIN

extern pyro::application* pyro::create_application();

/// \brief Adjusts the console to take 1/3rd of the monitor height.
void move_console();


int main(int argc, char** argv)
{
    move_console();

    pyro::logger::init();

    PYRO_PROFILE_BEGIN_SESSION("startup", "pyro_profile_startup.json");
    auto app = pyro::create_application();
    PYRO_PROFILE_END_SESSION();

    PYRO_PROFILE_BEGIN_SESSION("runtime", "pyro_profile_runtime.json");
    app->run();
    PYRO_PROFILE_END_SESSION();

    PYRO_PROFILE_BEGIN_SESSION("shutdown", "pyro_profile_shutdown.json");
    delete app;
    PYRO_PROFILE_END_SESSION();
    return 0;
}

void move_console()
{
#ifdef PYRO_DEBUG
    auto hwnd = GetConsoleWindow();
    SetWindowPos(hwnd, HWND_TOP, 0, static_cast<int>(1080 * 0.72), 1920, static_cast<int>(1080 * 0.25), SWP_NOZORDER);
    ShowWindow(hwnd, SW_SHOW);
#else
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
}

#else
    #error Pyro only supports Windows!
#endif