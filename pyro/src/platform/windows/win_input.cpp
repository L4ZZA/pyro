#include "pyro_pch.h"
#include "pyro/core/input.h"
#include "pyro/core/application.h"
#include "GLFW/glfw3.h"

namespace pyro
{
static std::pair<float, float> s_last_position;
static bool s_first_mouse = true;
}

bool pyro::input::key_pressed(int32_t key_code)
{
    auto window = application::window().native_window();
    auto state = glfwGetKey(static_cast<GLFWwindow*>(window), key_code);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool pyro::input::mouse_button_pressed(int32_t button)
{
    auto window = application::window().native_window();
    auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window), button);
    return state == GLFW_PRESS;
}

std::pair<float, float> pyro::input::mouse_position()
{
    auto& our_window = application::window();
    auto window = our_window.native_window();
    double x_pos, y_pos;
    glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x_pos, &y_pos);

    float x = static_cast<float>(x_pos);
    float y = static_cast<float>(y_pos);

    if(s_first_mouse)
    {
        s_last_position.first  = x;
        s_last_position.second = y;
        s_first_mouse = false;
    }

    if(!our_window.is_cursor_visible())
    {
        float delta_x = x - s_last_position.first;
        float delta_y = s_last_position.second - y; // reversed since y-coordinates range from bottom to top
        s_last_position.first  = x_pos;
        s_last_position.second = y_pos;
        return { delta_x, delta_y };
    }

    return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
}

float pyro::input::mouse_x()
{
    // c++ 17 way to assign std::pair(s)
    auto window = application::window().native_window();
    double x_pos, y_pos;
    glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x_pos, &y_pos);
    return static_cast<float>(x_pos);
}

float pyro::input::mouse_y()
{
    auto window = application::window().native_window();
    double x_pos, y_pos;
    glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x_pos, &y_pos);
    return static_cast<float>(y_pos);
}
