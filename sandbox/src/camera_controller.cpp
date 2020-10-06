#include "camera_controller.h"

void camera_controller_ortho::on_create()
{
    m_zoom_speed = 0.25f;
    m_zoom_level = 1.f;
    m_camera_component = &get_component<pyro::camera_component>();
}

void camera_controller_ortho::on_update(pyro::timestep const &ts)
{
    auto &transform = get_component<pyro::transform_component>().transform;
    float speed = 5.0f;

    if(pyro::input::key_pressed(pyro::key_codes::KEY_A)) // left
        transform[3][0] -= speed * ts;
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_D)) // right
        transform[3][0] += speed * ts;

    if(pyro::input::key_pressed(pyro::key_codes::KEY_W)) // up
        transform[3][1] += speed * ts;
    else if(pyro::input::key_pressed(pyro::key_codes::KEY_S)) // down
        transform[3][1] -= speed * ts;
}

void camera_controller_ortho::on_event(pyro::event &e)
{
    pyro::event_dispatcher dispatcher(e);
    dispatcher.dispatch<pyro::mouse_scrolled_event>(BIND_EVENT_FN(camera_controller_ortho::on_mouse_scrolled));
    dispatcher.dispatch<pyro::window_resize_event>(BIND_EVENT_FN(camera_controller_ortho::on_window_resized));
}

bool camera_controller_ortho::on_mouse_scrolled(pyro::mouse_scrolled_event &e)
{
    m_zoom_level -= e.y_offset() * m_zoom_speed;
    m_zoom_level = std::max(m_zoom_level, m_zoom_speed);
    m_camera_component->camera.orthographic_zoom_level(m_zoom_level);
 
    // returns if event is handled.
    return false;
}

bool camera_controller_ortho::on_window_resized(pyro::window_resize_event &e)
{
    m_width  = static_cast<float>(e.width()  * m_zoom_level); 
    m_height = static_cast<float>(e.height() * m_zoom_level);
    m_camera_component->camera.viewport_size((uint32_t)m_width, (uint32_t)m_height);
    // returns if event is handled.
    return false;
}
