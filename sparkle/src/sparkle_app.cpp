// --------- Entry Point ---------------
#include "pyro/core/entry_point.h"
#include "editor_layer.h"


class sparkle : public pyro::application
{
public:
    sparkle()
        : application("Sparkle editor")
    {
        m_editor_layer = std::make_shared<editor_layer>();
        push_layer(m_editor_layer);
    }

    ~sparkle() = default;

    void on_event(pyro::event &event) override
    {
        application::on_event(event);

        pyro::event_dispatcher dispatcher(event);
        // dispatch event on window X pressed 
        dispatcher.dispatch<pyro::key_pressed_event>(BIND_EVENT_FN(sparkle::on_key_pressed));
    }

    bool on_key_pressed(pyro::key_pressed_event &event)
    {
        if(event.event_type() == pyro::event_type_e::key_pressed)
        {
            if(event.key_code() == pyro::key_codes::KEY_ESCAPE)
            {
                exit();
            }
            //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
        }
        return false;
    }

private:
    std::shared_ptr<editor_layer> m_editor_layer;
};


pyro::application *pyro::create_application()
{
    return new sparkle();
}