#pragma once

#ifdef PYRO_PLATFORM_WIN

extern pyro::application* pyro::create_application();

int main(int argc, char** argv)
{
#ifdef PYRO_DEBUG
    auto hwnd = GetConsoleWindow();
    SetWindowPos(hwnd, HWND_TOP, 0, static_cast<int>(1080 * 0.72), 1920, static_cast<int>(1080 * 0.25), SWP_NOZORDER);
    ShowWindow(hwnd, SW_SHOW);
#else
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

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