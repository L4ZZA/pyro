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
    PYRO_CORE_WARN("Initialized logger");
    PYRO_INFO("Hello!");

    auto app = pyro::create_application();
    app->run();
    // TODO: uncomment following line and figure out why it crashes the program.
    //delete app;
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