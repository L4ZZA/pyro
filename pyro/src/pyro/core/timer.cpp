#include "pyro_pch.h"
#include "pyro/core/timer.h"
#include <GLFW/glfw3.h>

pyro::timer::timer()
{
    start();
}

void pyro::timer::start()
{
    m_started = true;
    // current time in seconds
    m_last_frame = glfwGetTime();
}

float pyro::timer::elapsed()
{
    if(!m_started)
        return 0.0;

    // current time in seconds
    double currentFrame = glfwGetTime();

    m_delta_time = currentFrame - m_last_frame;
    m_last_frame = currentFrame;

    return static_cast<float>(m_delta_time);
}

float pyro::timer::elapsed_millis()
{
    return elapsed() * 1000.f;
}
