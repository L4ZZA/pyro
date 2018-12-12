#include "pyro_pch.h"
#include "win_window.h"

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
    if(p_enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(1);

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

    if(!s_glfw_initialized)
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
}

void pyro::win_window::shut_down()
{
    glfwDestroyWindow(window_);
}
