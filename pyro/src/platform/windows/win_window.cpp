#include "pyro_pch.h"
#include "win_window.h"

#include "pyro/events/application_event.h"
#include "pyro/events/key_event.h"
#include "pyro/events/mouse_event.h"

#include "pyro/core/core.h"
#include "platform/opengl/gl_context.h"
#include "GLFW/glfw3.h"
#include "pyro/core/input.h"

//=============================================================================

namespace pyro
{
    static std::unordered_map<uint32_t, int32_t> s_key_repeat_count;
    static bool s_glfw_initialized = false;

    static void glfw_error_callback(int error_code, const char *description)
    {
        PYRO_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
    }
}

pyro::scope<pyro::window> pyro::window::create(window_props const &props)
{
    return make_scope<win_window>(props);
}

pyro::win_window::win_window(window_props const &props)
{
    PYRO_PROFILE_FUNCTION();
    init(props);
}

pyro::win_window::~win_window()
{
    PYRO_PROFILE_FUNCTION();
    shut_down();
}

void pyro::win_window::init(window_props const &props)
{
    PYRO_PROFILE_FUNCTION();
    m_data.title = props.m_title;
    m_data.width = props.m_width;
    m_data.height = props.m_height;
    m_data.last_mouse_x = m_data.width * 0.5f;
    m_data.last_mouse_y = m_data.height * 0.5f;

    PYRO_CORE_INFO("Creating window {0} [{1},{2}]", props.m_title, props.m_width, props.m_height);

    if(!s_glfw_initialized)
    {
        int32_t success = glfwInit();
        PYRO_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(glfw_error_callback);
        s_glfw_initialized = true;
    }

    m_window = glfwCreateWindow(
        static_cast<int>(props.m_width),
        static_cast<int>(props.m_height),
        props.m_title.c_str(),
        nullptr, nullptr);

    glfwMakeContextCurrent(m_window);

    m_graphics_context = new gl_context(m_window);
    m_graphics_context->init();

    // we're telling glfw to pass the window_data struct to all the defined callbacks
    // so that we can work with our defined data.
    glfwSetWindowUserPointer(m_window, &m_data);
    vsync(true);

    // GLFW callbacks
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int32_t width, int32_t height)
    {
        window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;

        window_resize_event event(width, height);
        data.event_callback(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window)
    {
        window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(window));
        window_closed_event event;
        data.event_callback(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(window));

        switch(action)
        {
            case GLFW_PRESS:
            {
                s_key_repeat_count[key] = 0;
                //PYRO_CORE_TRACE("Key {} pressed: {}", key, 0);
                key_pressed_event event(key, 0); // don't retrieve the value from s_key_repeat_count[key] as it will be less performant
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                s_key_repeat_count[key] = 0;
                //PYRO_CORE_TRACE("Key {} released: {}", key, 0);
                key_released_event event(key);
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                const int32_t repeats = ++s_key_repeat_count[key];
                key_pressed_event event(key, repeats);
                //PYRO_CORE_TRACE("Key {} repeats: {}", key, repeats);
                data.event_callback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(m_window, [](GLFWwindow *window, uint32_t key)
    {
        window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(window));

        key_typed_event event(key);
        data.event_callback(event);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
    {
        window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(window));

        switch(action)
        {
            case GLFW_PRESS:
            {
                mouse_button_pressed_event event(button);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                mouse_button_released_event event(button);
                data.event_callback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xOffset, double yOffset)
    {
        window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(window));
        mouse_scrolled_event event(static_cast<float>(xOffset), static_cast<float>(yOffset));
        data.event_callback(event);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double x_pos, double y_pos)
    {
        window_data &data = *static_cast<window_data *>(glfwGetWindowUserPointer(window));
        mouse_moved_event event(static_cast<float>(x_pos), static_cast<float>(y_pos));

        if(!data.is_mouse_cursor_visible)
        {
            const float delta_x = static_cast<float>(x_pos) - data.last_mouse_x;
            const float delta_y = data.last_mouse_y - static_cast<float>(y_pos);

            //PYRO_CORE_TRACE("Delta -> x: {0} | y: {1}", delta_x, delta_y);
            event = {delta_x, delta_y};
        }

        data.last_mouse_x = static_cast<float>(x_pos);
        data.last_mouse_y = static_cast<float>(y_pos);
        data.event_callback(event);
    });
}

void pyro::win_window::on_update()
{
    PYRO_PROFILE_FUNCTION();
    glfwPollEvents();
    m_graphics_context->swap_buffers();
}

void pyro::win_window::shut_down()
{
    PYRO_PROFILE_FUNCTION();
    s_key_repeat_count.clear();
    glfwDestroyWindow(m_window);
}

void pyro::win_window::vsync(bool enabled)
{
    PYRO_PROFILE_FUNCTION();
    if(enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_data.vsync = enabled;
}

bool pyro::win_window::vsync()
{
    return m_data.vsync;
}

void pyro::win_window::show_mouse_cursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_data.is_mouse_cursor_visible = true;
}

void pyro::win_window::hide_mouse_cursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_data.is_mouse_cursor_visible = false;
}

bool pyro::win_window::is_cursor_visible() const
{
    return m_data.is_mouse_cursor_visible;
}
