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

    static void glfw_error_callback(int p_error_code, const char * p_description)
    {
        PYRO_CORE_ERROR("GLFW Error ({0}): {1}", p_error_code, p_description);
    }
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
    glfwSwapBuffers(m_window);
}

void pyro::win_window::vsync(bool p_enabled)
{
    if (p_enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_data.vsync = p_enabled;
}

bool pyro::win_window::vsync()
{
    return m_data.vsync;
}

void pyro::win_window::init(window_props const& p_props)
{
    m_data.title = p_props.m_title;
    m_data.width = p_props.m_width;
    m_data.height = p_props.m_height;

    PYRO_CORE_INFO("Creating window {0} [{1},{2}]", p_props.m_title, p_props.m_width, p_props.m_height);

    if (!s_glfw_initialized)
    {
        int success = glfwInit();
        PYRO_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(glfw_error_callback);
        s_glfw_initialized = true;
    }

    m_window = glfwCreateWindow(
        static_cast<int>(p_props.m_width),
        static_cast<int>(p_props.m_height),
        p_props.m_title.c_str(),
        nullptr, nullptr);

    glfwMakeContextCurrent(m_window);
    // we're telling glfw to pass the window_data struct to all the defined callbacks
    // so that we ca work with our defined data.
    glfwSetWindowUserPointer(m_window, &m_data);
    vsync(true);


    // GLFW callbacks
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;

        window_resize_event event(width, height);
        data.event_callback(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        window_closed_event event;
        data.event_callback(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

        switch (action)
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

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
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

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        mouse_scrolled_event event(static_cast<float>(xOffset), static_cast<float>(yOffset));
        data.event_callback(event);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
    {
        window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
        mouse_moved_event event(static_cast<float>(xPos), static_cast<float>(yPos));
        data.event_callback(event);
    });
}

void pyro::win_window::shut_down()
{
    glfwDestroyWindow(m_window);
}
