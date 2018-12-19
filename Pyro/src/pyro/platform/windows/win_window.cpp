#include "pyro_pch.h"
#include "win_window.h"

#include "pyro/events/event.h"
#include "pyro/events/application_event.h"
#include "pyro/events/key_event.h"
#include "pyro/events/mouse_event.h"

//=============================================================================

namespace pyro
{
    static bool s_glfw_initialized = false;
}

pyro::window* pyro::window::create(window_props const& p_props)
{
    return new win_window(p_props);
}

pyro::win_window::win_window(window_props const& p_props)
{
    init(p_props);
}

pyro::win_window::~win_window()
{
    shut_down();
}

void pyro::win_window::on_update()
{
    glfwPollEvents();
    glfwSwapBuffers(window_);
}

void pyro::win_window::vsync(bool p_enabled)
{
    if (p_enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    data_.vsync_ = p_enabled;
}

bool pyro::win_window::vsync()
{
    return data_.vsync_;
}

void pyro::win_window::init(window_props const& p_props)
{
    data_.title_ = p_props.title_;
    data_.width_ = p_props.width_;
    data_.height_ = p_props.height_;

    PYRO_CORE_INFO("Creating window {0} [{1},{2}]", p_props.title_, p_props.width_, p_props.height_);

    if (!s_glfw_initialized)
    {
        int success = glfwInit();
        PYRO_CORE_ASSERT(success, "Could not initialize GLFW!");

        s_glfw_initialized = true;
    }

    window_ = glfwCreateWindow(
        static_cast<int>(p_props.width_),
        static_cast<int>(p_props.height_),
        p_props.title_.c_str(),
        nullptr, nullptr);

    glfwMakeContextCurrent(window_);
    // we're telling glfw to pass the window_data struct to all the defined callbacks
    // so that we ca work with our defined data.
    glfwSetWindowUserPointer(window_, &data_);
    vsync(true);


    // GLFW callbacks
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        data.width_ = width;
        data.height_ = height;

        window_resize_event event(width, height);
        data.event_callback(event);
    });

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        window_closed_event event;
        data.event_callback(event);
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

        switch (scancode)
        {
            case GLFW_PRESS:
            {
                key_pressed_event event(key, 0);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                key_released_event event(key);
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                key_pressed_event event(key, 1);
                data.event_callback(event);
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

        switch (action)
        {
            case GLFW_PRESS:
            {
                mouse_button_pressed_event event(button);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                mouse_button_release_event event(button);
                data.event_callback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        mouse_scrolled_event event(static_cast<float>(xOffset), static_cast<float>(yOffset));
        data.event_callback(event);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        mouse_moved_event event(static_cast<float>(xPos), static_cast<float>(yPos));
        data.event_callback(event);
    });
}

void pyro::win_window::shut_down()
{
    glfwDestroyWindow(window_);
}
