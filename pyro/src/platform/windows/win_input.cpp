#include "pyro_pch.h"
#include "win_input.h"
#include "pyro/application.h"
#include "GLFW/glfw3.h"

namespace pyro
{
    // some how we'll have to delete this?
    input* input::s_instance = new win_input();

    static bool s_first_mouse = true;
}

bool pyro::win_input::key_pressed_impl(int32_t key_code) const
{
    auto window = application::instance().get_window().native_window();
    auto state = glfwGetKey(static_cast<GLFWwindow*>(window), key_code);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool pyro::win_input::mouse_button_pressed_impl(int32_t button) const
{
    auto window = application::instance().get_window().native_window();
    auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window), button);
    return state == GLFW_PRESS;
}

std::pair<float, float> pyro::win_input::mouse_position_impl() const
{
    auto& our_window = application::instance().get_window();
    auto window = our_window.native_window();
    double x_pos, y_pos;
    glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x_pos, &y_pos);

    if(s_first_mouse)
    {
        s_last_position.first  = x_pos;
        s_last_position.second = y_pos;
        s_first_mouse = false;
    }

    float x = static_cast<float>(x_pos);
    float y = static_cast<float>(y_pos);
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

float pyro::win_input::mouse_x_impl() const
{
    // c++ 17 way to assign std::pair(s)
    auto window = application::instance().get_window().native_window();
    double x_pos, y_pos;
    glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x_pos, &y_pos);
    return static_cast<float>(x_pos);
}

float pyro::win_input::mouse_y_impl() const
{
    auto window = application::instance().get_window().native_window();
    double x_pos, y_pos;
    glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x_pos, &y_pos);
    return static_cast<float>(y_pos);
}
